#include <dci/couple/parser/errorInfo.hpp>
#include "../ast.hpp"
#include <algorithm>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class OwnerIndexer
        : public boost::static_visitor<>
    {
        std::vector<ErrorInfo> &_errs;

        SScope      *_scope     {nullptr};
        SStruct     *_struct    {nullptr};
        SVariant    *_variant   {nullptr};
        SInterface      *_interface     {nullptr};
        SMethod     *_method    {nullptr};
        SEnum       *_enum      {nullptr};
        SErrc       *_errc      {nullptr};

        template <class T>
        class CurrentSetter
        {
            T *&_storage;
            T *_originalValue;

        public:
            CurrentSetter(T *&storage, T *value)
                : _storage(storage)
                , _originalValue(storage)
            {
                _storage = value;
            }

            ~CurrentSetter()
            {
                _storage = _originalValue;
            }

        };

    public:
        OwnerIndexer(std::vector<ErrorInfo> &errs)
            : _errs(errs)
        {
        }

        void exec(const Scope &s)
        {
            CurrentSetter<SScope> css(_scope, s.get());
            exec(s->decls);
        }

    private:
        template <class V>
        void exec(std::vector<V> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const V &v)->void {boost::apply_visitor(*this, v);}
            );
        }

        void exec(std::vector<VariantField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const VariantField &v)->void {
                    v->owner = _variant;
                }
            );
        }

        void exec(std::vector<StructField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const StructField &v)->void {
                    v->owner = _struct;
                }
            );
        }

        void exec(std::vector<EnumField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const EnumField &v)->void {
                    v->owner = _enum;
                }
            );
        }

        void exec(std::vector<ErrcField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const ErrcField &v)->void {
                    v->owner = _errc;
                }
            );
        }

        void exec(std::vector<Method> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const Method &v)->void {
                    v->owner = _interface;

                    CurrentSetter<SMethod> csm(_method, v.get());
                    exec(v->query);
                }
            );
        }

        void exec(std::vector<MethodParam> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const MethodParam &v)->void {
                    v->owner = _method;
                }
            );
        }

    public:
        template <class T>
        void operator()(T &v)
        {
            (*this)(v.get());
        }

        void operator()(SAlias *v)
        {
            v->owner = _scope;
            _scope->aliases.insert(std::make_pair(v->name->value, v));
        }

        void operator()(SVariant *v)
        {
            v->owner = _scope;
            _scope->variants.insert(std::make_pair(v->name->value, v));
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SVariant> csv(_variant, v);
            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SStruct *v)
        {
            v->owner = _scope;
            _scope->structs.insert(std::make_pair(v->name->value, v));
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SStruct> csst(_struct, v);
            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SEnum *v)
        {
            v->owner = _scope;
            _scope->enums.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SEnum> cse(_enum, v);
            exec(v->fields);
        }

        void operator()(SErrc *v)
        {
            v->owner = _scope;
            _scope->errcs.insert(std::make_pair(v->name->value, v));

            {
                auto errPusher = [&](SScopeEntry *place)
                {
                    _errs.emplace_back(ErrorInfo {
                                          v->name->pos.begin().file(),
                                          static_cast<int>(v->name->pos.begin().line()),
                                          static_cast<int>(v->name->pos.begin().column()),
                                          "errc can be placed only in pure scope"});

                    _errs.emplace_back(ErrorInfo {
                                          place->name->pos.begin().file(),
                                          static_cast<int>(place->name->pos.begin().line()),
                                          static_cast<int>(place->name->pos.begin().column()),
                                          "declaration of non pure scope "+place->name->value});
                };

                if(_interface)
                {
                    errPusher(_interface);
                }
                if(_struct)
                {
                    errPusher(_struct);
                }
                if(_variant)
                {
                    errPusher(_variant);
                }
            }

            CurrentSetter<SErrc> cse(_errc, v);
            exec(v->fields);
        }

        void operator()(SInterface *v)
        {
            v->owner = _scope;
            _scope->interfaces.insert(std::make_pair(v->name->value, v));
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SInterface> csi(_interface, v);
            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
            exec(v->methods);
        }

        void operator()(SScope *v)
        {
            v->owner = _scope;
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
        }
    };
}}}}}
