#include <dci/couple/parser/errorInfo.hpp>
#include "../ast.hpp"
#include <vector>
#include <map>
#include <algorithm>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class BasesChecker
        : public boost::static_visitor<bool>
    {
        std::vector<ErrorInfo> &_errs;

    public:
        BasesChecker(std::vector<ErrorInfo> &errs)
            : _errs(errs)
        {}

    public:
        bool exec(const Scope &s)
        {
            return exec(s->decls);
        }

    private:
        bool exec(const std::vector<Decl> &decls)
        {
            return std::accumulate(
                decls.begin(),
                decls.end(),
                true,
                [&](bool v, const Decl &d)->bool{return boost::apply_visitor(*this, d) && v;}
            );
        }

    public:
        template <class T>
        bool operator()(const T &v)
        {
            bool res = true;
            res &= check(v.get());
            res &= walk(v.get());
            return res;
        }

    private:
        bool check(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::bases)!=0, bool>::type check(T *v)
        {
            if(!v->bases)
            {
                return true;
            }

            bool res = true;

            //duplicate direct bases
            const auto &instances = v->bases->instances;
            const auto &scopedNames = v->bases->scopedNames;

            for(size_t idx1(1); idx1<instances.size(); ++idx1)
            {
                for(size_t idx2(0); idx2<idx1; ++idx2)
                {
                    if(instances[idx1] == instances[idx2])
                    {
                        const ScopedName &scopedName1 = scopedNames[idx1];
                        _errs.emplace_back(ErrorInfo {
                                              scopedName1->pos.begin().file(),
                                              static_cast<int>(scopedName1->pos.begin().line()),
                                              static_cast<int>(scopedName1->pos.begin().column()),
                                              "duplicate base type: "+scopedName1->toString()});

                        const ScopedName &scopedName2 = scopedNames[idx2];
                        _errs.emplace_back(ErrorInfo {
                                              scopedName2->pos.begin().file(),
                                              static_cast<int>(scopedName2->pos.begin().line()),
                                              static_cast<int>(scopedName2->pos.begin().column()),
                                              "previous base type: "+scopedName2->toString()});
                        res = false;
                    }
                }
            }

            //cycles in all bases
            std::set<T*> allBases;
            res &= collectAllBases(v, v->bases->instances, allBases);

            return res;
        }

    private:
        template <class T>
        bool collectAllBases(T *v, const std::vector<T*> &bases, std::set<T*> &allBases)
        {
            if(allBases.end() != allBases.find(v))
            {
                _errs.emplace_back(ErrorInfo {
                                      v->name->pos.begin().file(),
                                      static_cast<int>(v->name->pos.begin().line()),
                                      static_cast<int>(v->name->pos.begin().column()),
                                      "cycle in inheritance: "+v->name->value});
                return false;
            }

            allBases.insert(bases.begin(), bases.end());

            for(auto base : bases)
            {
                if(base->bases && !collectAllBases(v, base->bases->instances, allBases))
                {
                    return false;
                }
            }

            return true;
        }

    private:
        bool walk(...)
        {
            return true;
        }

        bool walk(SScope *v)
        {
            return exec(v->decls);
        }

    };

}}}}}
