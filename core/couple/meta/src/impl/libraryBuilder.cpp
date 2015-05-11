#include "libraryBuilder.hpp"
#include "library.hpp"

#include <type_traits>

namespace dci { namespace couple { namespace meta { namespace impl
{
    LibraryBuilder::LibraryBuilder()
    {
    }

    LibraryBuilder::~LibraryBuilder()
    {
        rollbackChanges();
    }

    void LibraryBuilder::merge(const Library &lib)
    {
        (void)lib;
        assert(0);
    }

    Primitive *LibraryBuilder::addPrimitive()
    {
        return _lc.create<Primitive>();
    }

    List *LibraryBuilder::addList()
    {
        return _lc.create<List>();
    }

    Set *LibraryBuilder::addSet()
    {
        return _lc.create<Set>();
    }

    Map *LibraryBuilder::addMap()
    {
        return _lc.create<Map>();
    }

    Ptr *LibraryBuilder::addPtr()
    {
        return _lc.create<Ptr>();
    }

    Array *LibraryBuilder::addArray()
    {
        return _lc.create<Array>();
    }

    Scope *LibraryBuilder::addScope(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Scope>();
        res->setName(name);

        if(parent)
        {
            _lc.checkPresense(parent);
            parent->add(res);
            res->setScope(parent);
        }

        return res;
    }

    Alias *LibraryBuilder::addAlias(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Alias>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);
        res->setScope(parent);

        return res;
    }

    Struct *LibraryBuilder::addStruct(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Struct>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);
        res->setScope(parent);

        return res;
    }

    Variant *LibraryBuilder::addVariant(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Variant>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);
        res->setScope(parent);

        return res;
    }

    Enum *LibraryBuilder::addEnum(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Enum>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);
        res->setScope(parent);

        return res;
    }

    Iface *LibraryBuilder::addIface(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Iface>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);
        res->setScope(parent);

        return res;
    }

    Method *LibraryBuilder::addMethod(Iface *parent, const std::string &name)
    {
        auto res = _lc.create<Method>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Struct *parent, const std::string &name)
    {
        auto res = _lc.create<Attribute>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Variant *parent, const std::string &name)
    {
        auto res = _lc.create<Attribute>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    Attribute *LibraryBuilder::addAttribute(Method *parent, const std::string &name)
    {
        auto res = _lc.create<Attribute>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    EnumValue *LibraryBuilder::addEnumValue(Enum *parent, const std::string &name)
    {
        auto res = _lc.create<EnumValue>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);

        return res;
    }

    void LibraryBuilder::addBase(Struct *target, Struct *base)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(base);
        target->addBase(base);
    }

    void LibraryBuilder::addBase(Iface *target, Iface *base)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(base);
        target->addBase(base);
    }

    void LibraryBuilder::setType(Attribute *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setType(type);
    }

    void LibraryBuilder::setType(Alias *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setType(type);
    }

    void LibraryBuilder::setElementType(Set *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(List *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Array *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Ptr *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->setElementType(type);
    }

    void LibraryBuilder::setElementType(Map *target, Type *keyType, Type *valueType)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(keyType);
        _lc.checkPresense(valueType);
        target->setElementType1(keyType);
        target->setElementType2(valueType);
    }

    void LibraryBuilder::setMethodDirection(Method *target, CallDirection direction)
    {
        _lc.checkPresense(target);
        target->setDirection(direction);
    }

    void LibraryBuilder::addReplyArg(Method *target, Type *type)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(type);
        target->add(type);
    }

    void LibraryBuilder::setMethodNoreply(Method *target, bool noreply)
    {
        _lc.checkPresense(target);
        target->setNoreply(noreply);
    }

    void LibraryBuilder::setArraySize(Array *target, std::uint32_t size)
    {
        _lc.checkPresense(target);
        target->setSize(size);
    }

    void LibraryBuilder::setPrimitiveKind(Primitive *target, PrimitiveKind kind)
    {
        _lc.checkPresense(target);
        target->setKind(kind);
    }

    bool LibraryBuilder::commitChanges(Library &lib, std::vector<CommitError> &errors)
    {
        (void)errors;
        //TODO: check errors

        //give a names to unnamed method arguments
        {
            for(Iface *i : _lc.items<Iface>())
            {
                for(const Method *cm : i->Compound<Method>::elements())
                {
                    std::set<std::string> usedNames;
                    usedNames.insert(std::string());
                    std::size_t indexGen = 0;

                    for(const Attribute *ca : cm->Compound<Attribute>::elements())
                    {
                        Attribute *a = const_cast<Attribute *>(ca);
                        while(usedNames.end() != usedNames.find(a->name()))
                        {
                            a->setName("unnamed_"+std::to_string(indexGen++));
                        }
                        usedNames.insert(a->name());
                    }
                }
            }
        }

        //create opposite ifaces
        {
            auto ifaces = _lc.items<Iface>();
            for(Iface *v : ifaces)
            {
                Iface *o = addIface(const_cast<Scope *>(v->scope()), v->name()+"Opposite");

                v->setPrimary(true);
                v->setOpposite(o);

                o->setPrimary(false);
                o->setOpposite(v);

                fillOppositeIfaceScope(o, v);

                for(const Method *m : v->Compound<Method>::elements())
                {
                    Method *om = addMethod(o, m->name());

                    setMethodDirection(om, CallDirection::in == m->direction() ? CallDirection::out : CallDirection::in);
                    setMethodNoreply(om, m->noreply());

                    for(const Attribute *a : m->Compound<Attribute>::elements())
                    {
                        Attribute *oa = addAttribute(om, a->name());
                        setType(oa, const_cast<Type *>(a->type()));
                    }

                    for(const Type *t : m->Compound<Type>::elements())
                    {
                        addReplyArg(om, const_cast<Type *>(t));
                    }
                }
            }


            for(Iface *v : ifaces)
            {
                Iface *o = const_cast<Iface *>(v->opposite());
                for(const Iface *b : v->bases())
                {
                    addBase(o, const_cast<Iface *>(b->opposite()));
                }
            }
        }

        lib = std::move(_lc);
        _lc.clear();
        return true;
    }

    void LibraryBuilder::rollbackChanges()
    {
        _lc.clear();
    }

    void LibraryBuilder::fillOppositeIfaceScope(Scope *o, Scope *v)
    {
        for(auto *c : v->Compound<Struct>::elements())  setType(addAlias(o, c->name()), const_cast<Struct *>(c));
        for(auto *c : v->Compound<Variant>::elements()) setType(addAlias(o, c->name()), const_cast<Variant *>(c));
        for(auto *c : v->Compound<Enum>::elements())    setType(addAlias(o, c->name()), const_cast<Enum *>(c));
        for(auto *c : v->Compound<Alias>::elements())   setType(addAlias(o, c->name()), const_cast<Alias *>(c));
        for(auto *c : v->Compound<Iface>::elements())   setType(addAlias(o, c->name()), const_cast<Iface *>(c));

        for(auto *c : v->Compound<Scope>::elements())
        {
            Scope *s = addScope(o, c->name());
            fillOppositeIfaceScope(s, const_cast<Scope *>(c));
        }
    }

}}}}
