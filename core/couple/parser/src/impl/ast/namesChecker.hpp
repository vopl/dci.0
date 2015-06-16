#include <dci/couple/parser/errorInfo.hpp>
#include "../ast.hpp"
#include <vector>
#include <map>
#include <algorithm>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class NamesChecker
        : public boost::static_visitor<bool>
    {
        std::vector<ErrorInfo> &_errs;

        std::map<std::string, Name> _names;
        std::map<std::string, Name> _namesMethodIn;
        std::map<std::string, Name> _namesMethodOut;

    public:
        NamesChecker(std::vector<ErrorInfo> &errs)
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
            res &= checkName(v.get());
            res &= checkFields(v.get());
            res &= checkQuery(v.get());
            res &= checkChildren(v.get());
            res &= checkOpposite(v.get());
            return res;
        }

    private:
        bool checkName(...)
        {
            return true;
        }

        bool checkName(const SMethod *v)
        {
            const Name &cur = v->name;

            std::map<std::string, Name> &names = MethodDirection::in==v->direction ? _namesMethodIn : _namesMethodOut;
            auto ires = names.insert(std::make_pair(cur->value, cur));
            if(!ires.second)
            {
                _errs.emplace_back(ErrorInfo {
                                      cur->pos.begin().file(),
                                      static_cast<int>(cur->pos.begin().line()),
                                      static_cast<int>(cur->pos.begin().column()),
                                      "duplicate name: "+cur->value});

                const Name &prev = ires.first->second;

                _errs.emplace_back(ErrorInfo {
                                      prev->pos.begin().file(),
                                      static_cast<int>(prev->pos.begin().line()),
                                      static_cast<int>(prev->pos.begin().column()),
                                      "previous declaration of "+prev->value});

                return false;
            }

            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::name)!=0, bool>::type checkName(const T *v)
        {
            const Name &cur = v->name;
            auto ires = _names.insert(std::make_pair(cur->value, cur));
            if(!ires.second)
            {
                _errs.emplace_back(ErrorInfo {
                                      cur->pos.begin().file(),
                                      static_cast<int>(cur->pos.begin().line()),
                                      static_cast<int>(cur->pos.begin().column()),
                                      "duplicate name: "+cur->value});

                const Name &prev = ires.first->second;

                _errs.emplace_back(ErrorInfo {
                                      prev->pos.begin().file(),
                                      static_cast<int>(prev->pos.begin().line()),
                                      static_cast<int>(prev->pos.begin().column()),
                                      "previous declaration of "+prev->value});

                return false;
            }

            return true;
        }

    private:

        template <class T>
        static auto getFieldsOrQuery(const T *v)
        {
            return v->fields;
        }

        static auto getFieldsOrQuery(const SMethod *v)
        {
            return v->query;
        }

        template <class T>
        bool checkFieldsOrQuery(const T *v)
        {
            std::map<std::string, Name> names;

            bool res = true;
            for(const auto &f : getFieldsOrQuery(v))
            {
                const Name &cur = f->name;

                if(cur)
                {
                    auto ires = names.insert(std::make_pair(cur->value, cur));
                    if(!ires.second)
                    {
                        _errs.emplace_back(ErrorInfo {
                                              cur->pos.begin().file(),
                                              static_cast<int>(cur->pos.begin().line()),
                                              static_cast<int>(cur->pos.begin().column()),
                                              "duplicate name: "+cur->value});

                        const Name &prev = ires.first->second;

                        _errs.emplace_back(ErrorInfo {
                                              prev->pos.begin().file(),
                                              static_cast<int>(prev->pos.begin().line()),
                                              static_cast<int>(prev->pos.begin().column()),
                                              "previous declaration of "+prev->value});
                        res = false;
                    }
                }
            }

            return res;
        }

    private:
        bool checkFields(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::fields)!=0, bool>::type checkFields(const T *v)
        {
            return checkFieldsOrQuery(v);
        }

    private:
        bool checkQuery(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<sizeof(T::query)!=0, bool>::type checkQuery(const T *v)
        {
            return checkFieldsOrQuery(v);
        }

    private:
        bool checkChildren(...)
        {
            return true;
        }

        template <class T>
        typename std::enable_if<std::is_base_of<SScope, T>::value, bool>::type checkChildren(const T *v)
        {
            return NamesChecker(_errs).exec(v->decls);
        }

        bool checkChildren(const SInterface *v)
        {
            NamesChecker checker(_errs);

            return std::accumulate(
                v->methods.begin(),
                v->methods.end(),
                checker.exec(v->decls),
                [&](bool v, const Method &d)->bool{
                    return checker(d) && v;
                });
        }

    private:
        bool checkOpposite(...)
        {
            return true;
        }

        bool checkOpposite(const SInterface *v)
        {
            const Name &name = v->name;
            if(name->value.size()>=8 && name->value.size()-8 == name->value.find("Opposite"))
            {
                _errs.emplace_back(ErrorInfo {
                                      name->pos.begin().file(),
                                      static_cast<int>(name->pos.begin().line()),
                                      static_cast<int>(name->pos.begin().column()),
                                      "'Opposite' suffix is not allowed for interfaces"});
                return false;
            }

            return true;
        }
    };

}}}}}
