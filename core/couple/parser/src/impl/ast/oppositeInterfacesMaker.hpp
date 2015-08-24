#include <dci/couple/parser/errorInfo.hpp>
#include "../ast.hpp"
#include <algorithm>
#include <map>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class OppositeInterfacesMaker
        : public boost::static_visitor<Interface>
    {
    public:
        void exec(const Scope &s)
        {
            exec(s->decls);
        }

    private:
        void exec(std::vector<Decl> &decls)
        {
            std::vector<Decl> declsCopy(decls);

            std::for_each(
                declsCopy.begin(),
                declsCopy.end(),
                [&](const Decl &v)->void {
                    Interface opposite = boost::apply_visitor(*this, v);
                    if(opposite)
                    {
                        decls.push_back(opposite);
                    }
                }
            );
        }

    public:
        Interface operator()(const Scope &v)
        {
            exec(v->decls);
            return Interface();
        }
        Interface operator()(const Struct &v)
        {
            exec(v->decls);
            return Interface();
        }
        Interface operator()(const Variant &v)
        {
            exec(v->decls);
            return Interface();
        }
        Interface operator()(const Interface &v)
        {
            exec(v->decls);

            Interface ov(new SInterface);

            ov->name.reset(new SName);
            ov->name->pos = v->name->pos;
            ov->name->value = v->name->value+"Opposite";

            ov->primary = false;
            ov->opposite = v;
            v->opposite = ov;

            if(v->bases)
            {
                ov->bases.reset(new SBaseInterfaces);

                for(const ScopedName &sn : v->bases->scopedNames)
                {
                    ScopedName osn(new SScopedName);
                    osn->pos = sn->pos;
                    osn->root = sn->root;
                    osn->values = sn->values;
                    if(!osn->values.empty())
                    {
                        osn->values.back().reset(new SName);
                        osn->values.back()->pos = sn->values.back()->pos;
                        osn->values.back()->value = sn->values.back()->value+"Opposite";
                    }

                    ov->bases->scopedNames.push_back(osn);
                }
            }

            for(const Method &m : v->methods)
            {
                Method om(new SMethod);
                om->direction = MethodDirection::out == m->direction ? MethodDirection::in : MethodDirection::out;
                om->name = m->name;

                for(const MethodParam &mp : m->query)
                {
                    MethodParam omp(new SMethodParam);
                    omp->name = mp->name;
                    omp->type = copyTypeUse(mp->type);
                    om->query.push_back(omp);
                }

                om->noreply = m->noreply;
                for(TypeUse &tu : m->reply)
                {
                    om->reply.push_back(copyTypeUse(tu));
                }

                ov->methods.push_back(om);
            }

            fillOppositeInterfaceScope(ov, v);

            return ov;
        }

        template <class T>
        Interface operator()(const T &)
        {
            return Interface();
        }

        void fillOppositeInterfaceScope(const Scope &ov, const Scope &v)
        {
            for(const Decl &d : v->decls)
            {
                //Alias
                if(boost::get<Alias>(&d))
                {
                    const Alias &a = boost::get<Alias>(d);
                    Alias oa(new SAlias);
                    oa->name = a->name;
                    oa->type = copyTypeUse(a->type);
                    ov->decls.push_back(oa);
                }

                auto aliasMaker = [&](auto *pch)
                {
                    if(!pch)
                    {
                        return;
                    }
                    auto ch = *pch;

                    Alias oa(new SAlias);
                    oa->name = ch->name;

                    ScopedName sn(new SScopedName);
                    sn->asDecl = ch.get();
                    sn->asScopedEntry = ch.get();
                    oa->type = copyTypeUse(sn);
                    ov->decls.push_back(oa);
                };

                aliasMaker(boost::get<Variant>(&d));
                aliasMaker(boost::get<Struct>(&d));
                aliasMaker(boost::get<Enum>(&d));
                aliasMaker(boost::get<Errc>(&d));
                aliasMaker(boost::get<Interface>(&d));

                //Scope
                if(boost::get<Scope>(&d))
                {
                    const Scope &s = boost::get<Interface>(d);
                    Scope os(new SScope);
                    os->name = v->name;
                    ov->decls.push_back(os);

                    fillOppositeInterfaceScope(os, s);
                }

            }

        }

        TypeUse copyTypeUse(TypeUse tu)
        {
            auto typeUseCopyMaker = [&](auto *pch, TypeUse &res)
            {
                std::remove_reference_t<decltype(*pch)> och;
                if(!pch)
                {
                    return;
                }

                och.reset(new std::remove_reference_t<decltype(**pch)>);
                *och = **pch;
                res = och;
            };

            TypeUse res;

            typeUseCopyMaker(boost::get<Primitive>(&tu), res);
            typeUseCopyMaker(boost::get<List>(&tu), res);
            typeUseCopyMaker(boost::get<Set>(&tu), res);
            typeUseCopyMaker(boost::get<Map>(&tu), res);
            typeUseCopyMaker(boost::get<Ptr>(&tu), res);
            typeUseCopyMaker(boost::get<Array>(&tu), res);
            typeUseCopyMaker(boost::get<ScopedName>(&tu), res);

            return res;
        }
    };
}}}}}
