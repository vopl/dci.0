#include <dci/couple/parser/errorInfo.hpp>
#include "../ast.hpp"
#include <vector>
#include <algorithm>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class ScopedNamesResolver
            : public boost::static_visitor<bool>
    {
        std::vector<ErrorInfo> &_errs;
        SScope *_rootScope{nullptr};
        SScope *_currentScope{nullptr};
        SInterface *_currentInterface{nullptr};
        int _oppositeInterfaceLevel{0};
        int _resolvedCount{0};
        bool _reportErrors{false};

    public:
        ScopedNamesResolver(std::vector<ErrorInfo> &errs)
            : _errs(errs)
        {
        }

    public:
        bool exec(const Scope &s)
        {
            bool res = false;
            do
            {
                assert(!_rootScope);
                _rootScope = s.get();
                _currentScope = _rootScope;
                _oppositeInterfaceLevel = 0;
                _resolvedCount = 0;
                _reportErrors = false;


                res = exec(s->decls);

                _currentScope = nullptr;
                _rootScope = nullptr;
            }
            while(!res && _resolvedCount);

            if(!res)
            {
                assert(!_rootScope);
                _rootScope = s.get();
                _currentScope = _rootScope;
                _oppositeInterfaceLevel = 0;
                _resolvedCount = 0;
                _reportErrors = true;


                res = exec(s->decls);

                _currentScope = nullptr;
                _rootScope = nullptr;

                assert(!res);
            }

            return res;
        }

    private:
        template <class V>
        bool exec(std::vector<V> &vs)
        {
            return std::accumulate(
                vs.begin(),
                vs.end(),
                true,
                [&](bool state, const V &v)->bool {return boost::apply_visitor(*this, v) && state;}
            );
        }

        bool exec(std::vector<EnumField> &)
        {
            return true;
        }

        bool exec(std::vector<ErrcField> &)
        {
            return true;
        }

        bool exec(std::vector<VariantField> &vs)
        {
            return std::accumulate(
                vs.begin(),
                vs.end(),
                true,
                [&](bool state, const VariantField &v)->bool {
                    return resolve(v->owner, v->type) && state;
                }
            );
        }

        bool exec(std::vector<StructField> &vs)
        {
            return std::accumulate(
                vs.begin(),
                vs.end(),
                true,
                [&](bool state, const StructField &v)->bool {
                    return resolve(v->owner, v->type) && state;
                }
            );
        }

        bool exec(std::vector<Method> &vs)
        {
            return std::accumulate(
                vs.begin(),
                vs.end(),
                true,
                [&](bool state, const Method &v)->bool {
                    state &= exec(v->query);
                    state &= std::accumulate(
                                    v->reply.begin(),
                                    v->reply.end(),
                                    state,
                                    [&](bool lstate, TypeUse &t)->bool {
                                        return resolve(v->owner, t) && lstate;
                                    }
                                );
                    return state;
                }
            );
        }

        bool exec(std::vector<MethodParam> &vs)
        {
            return std::accumulate(
                vs.begin(),
                vs.end(),
                true,
                [&](bool state, const MethodParam &v)->bool {
                    return resolve(v->owner->owner, v->type) && state;
                }
            );
        }

    private:
        template <class T>
        bool resolveOne(const std::map<std::string, T> &map, const std::string &name, SScopedName &scopedName)
        {
            auto target = map.find(name);

            if(map.end() == target)
            {
                return false;
            }

            if(!scopedName.asScopedEntry)
            {
                _resolvedCount++;
                scopedName.asDecl = target->second;
                scopedName.asScopedEntry = target->second;
            }

            assert(scopedName.asScopedEntry == target->second);
            return true;
        }

        bool resolveOne(const SScope *scope, SScopedName &scopedName)
        {
            auto iter = scopedName.values.begin();
            auto last = --scopedName.values.end();

            bool failed = false;
            for(; iter!=last; ++iter)
            {
                const Name &name = *iter;
                auto scopeIter = scope->scopes.find(name->value);

                if(scope->scopes.end() == scopeIter)
                {
                    failed = true;
                    break;
                }
                scope = scopeIter->second;
            }

            if(!failed)
            {
                const Name &name = *last;
                const std::string &nameValue = name->value;

                if(resolveOne(scope->aliases, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->structs, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->variants, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->enums, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->errcs, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->interfaces, nameValue, scopedName))
                {
                    return true;
                }
            }

            if(scope->owner)
            {
                return resolveOne(scope->owner, scopedName);
            }

            return false;
        }

        template <class T>
        typename std::enable_if<sizeof(T::bases)!=0, bool>::type resolveOne(const T *scope, SScopedName &scopedName)
        {
            if(resolveOne(static_cast<const SScope*>(scope), scopedName))
            {
                return true;
            }

            if(scope->bases)
            {
                assert(!scope->bases->instances.size() || scope->bases->instances.size() == scope->bases->scopedNames.size());

                for(const T *b : scope->bases->instances)
                {
                    if(b && resolveOne(b, scopedName))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        bool resolve(const SScope *scope, SScopedName &scopedName)
        {
            if(scopedName.values.empty() && scopedName.asScopedEntry)
            {
                //already resolved
                return true;
            }
            assert(!scopedName.values.empty());

            if(scopedName.root)
            {
                if(resolveOne(_rootScope, scopedName))
                {
                    return true;
                }

                if(_reportErrors && !_oppositeInterfaceLevel)
                {
                    _errs.emplace_back(ErrorInfo {
                                          scopedName.pos.begin().file(),
                                          static_cast<int>(scopedName.pos.begin().line()),
                                          static_cast<int>(scopedName.pos.begin().column()),
                                          "unable to resolve type name: " + scopedName.toString()});
                }

                return false;
            }

            if(_currentInterface)
            {
                if(resolveOne(_currentInterface, scopedName))
                {
                    return true;
                }
            }

            if(resolveOne(scope, scopedName))
            {
                return true;
            }

            if(_reportErrors && !_oppositeInterfaceLevel)
            {
                _errs.emplace_back(ErrorInfo {
                                      scopedName.pos.begin().file(),
                                      static_cast<int>(scopedName.pos.begin().line()),
                                      static_cast<int>(scopedName.pos.begin().column()),
                                      "unable to resolve type name: " + scopedName.toString()});
            }

            return false;
        }

        bool resolve(const SScope *scope, TypeUse &typeUse)
        {
            ScopedName *sn = boost::get<ScopedName>(&typeUse);
            if(!sn)
            {
                return boost::apply_visitor(*this, typeUse);
            }
            assert(*sn);

            return resolve(scope, *sn->get());
        }

    public:
        template <class T>
        bool operator()(const T &v)
        {
            bool res = true;

            SScope *outerScope = nullptr;

            res &= beginResolveScope(v.get(), outerScope);
            res &= resolveBases(v.get());
            res &= resolveFields(v.get());

            res &= resolveAlias(v.get());

            res &= resolveElementType(v.get());
            res &= resolveValueType(v.get());
            res &= resolveKeyType(v.get());

            endResolveScope(outerScope);

            return res;
        }

        bool operator()(const Interface &v)
        {
            if(!v->primary)
            {
                _oppositeInterfaceLevel++;
            }

            bool res = true;

            SScope *outerScope = nullptr;

            res &= beginResolveScope(v.get(), outerScope);
            res &= resolveBases(v.get());
            res &= resolveFields(v.get());
            res &= resolveMethods(v.get());

            endResolveScope(outerScope);

            if(!v->primary)
            {
                _oppositeInterfaceLevel--;
            }

            return res;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool beginResolveScope(...)
        {
            return true;
        }

        bool beginResolveScope(SScope *v, SScope *&outerScope)
        {
            outerScope = _currentScope;
            _currentScope = v;
            bool res = exec(v->decls);

            return res;
        }

        bool beginResolveScope(SInterface *v, SScope *&outerScope)
        {
            outerScope = _currentScope;
            _currentScope = v;
            _currentInterface = v;
            bool res = exec(v->decls);

            return res;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void endResolveScope(...)
        {
        }

        void endResolveScope(SScope *v, SScope *outerScope)
        {
            (void)v;
            _currentScope = outerScope;
        }
        void endResolveScope(SInterface *v, SScope *outerScope)
        {
            (void)v;
            _currentScope = outerScope;
            _currentInterface = static_cast<SInterface*>(outerScope);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool resolveBases(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::bases)!=0, bool>::type resolveBases(T *v)
        {
            bool res = true;
            if(v->bases)
            {
                const auto &scopedNames = v->bases->scopedNames;
                auto &instances = v->bases->instances;

                instances.resize(scopedNames.size());

                for(std::size_t i(0); i<scopedNames.size(); ++i)
                {
                    if(instances[i])
                    {
                        continue;
                    }

                    const ScopedName &sn = scopedNames[i];

                    bool localRes = resolve(v->owner, *sn);
                    if(!localRes)
                    {
                        res = false;
                    }
                    else if((!boost::get<T*>(&sn->asDecl)))
                    {
                        if(_reportErrors && !_oppositeInterfaceLevel)
                        {
                            _errs.emplace_back(ErrorInfo {
                                                  sn->pos.begin().file(),
                                                  static_cast<int>(sn->pos.begin().line()),
                                                  static_cast<int>(sn->pos.begin().column()),
                                                  "resolved type name is incompatible: " + sn->toString()});
                        }
                        res = false;
                    }
                    else
                    {
                        _resolvedCount++;
                        instances[i] = boost::get<std::remove_reference_t<decltype(v->bases->instances[0])>>(sn->asDecl);
                    }

                }
            }

            return res;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool resolveFields(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::fields)!=0, bool>::type resolveFields(T *v)
        {
            return exec(v->fields);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool resolveMethods(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::methods)!=0, bool>::type resolveMethods(T *v)
        {
            return exec(v->methods);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool resolveAlias(...)
        {
            return true;
        }

        bool resolveAlias(SAlias *v)
        {
            return resolve(v->owner, v->type);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool resolveElementType(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::elementType)!=0, bool>::type resolveElementType(T *v)
        {
            return resolve(_currentScope, v->elementType);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool resolveValueType(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::valueType)!=0, bool>::type resolveValueType(T *v)
        {
            return resolve(_currentScope, v->valueType);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool resolveKeyType(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::keyType)!=0, bool>::type resolveKeyType(T *v)
        {
            return resolve(_currentScope, v->keyType);
        }
    };

}}}}}
