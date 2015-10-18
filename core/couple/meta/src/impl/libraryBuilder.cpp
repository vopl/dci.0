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
        assert(0&&"not implemented yet");
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

    Errc *LibraryBuilder::addErrc(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Errc>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);
        res->setScope(parent);

        return res;
    }

    Interface *LibraryBuilder::addInterface(Scope *parent, const std::string &name)
    {
        auto res = _lc.create<Interface>();
        res->setName(name);

        _lc.checkPresense(parent);
        parent->add(res);
        res->setScope(parent);

        return res;
    }

    Method *LibraryBuilder::addMethod(Interface *parent, const std::string &name)
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

    ErrcValue *LibraryBuilder::addErrcValue(Errc *parent, const std::string &name)
    {
        auto res = _lc.create<ErrcValue>();
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

    void LibraryBuilder::addBase(Interface *target, Interface *base)
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

    void LibraryBuilder::setDescription(ErrcValue *target, const std::string &description)
    {
        _lc.checkPresense(target);
        target->setDescription(description);
    }

    void LibraryBuilder::setValue(EnumValue *target, const std::string &value)
    {
        _lc.checkPresense(target);
        target->setValue(value);
    }

    void LibraryBuilder::setInterfacePrimary(Interface *target, bool primary)
    {
        _lc.checkPresense(target);
        target->setPrimary(primary);
    }

    void LibraryBuilder::setInterfaceOpposite(Interface *target, Interface *opposite)
    {
        _lc.checkPresense(target);
        _lc.checkPresense(opposite);
        target->setOpposite(opposite);
    }

    bool LibraryBuilder::commitChanges(Library &lib, std::vector<CommitError> &errors)
    {
        (void)errors;
        //TODO: check errors

        //give a names to unnamed method arguments
        {
            for(Interface *i : _lc.items<Interface>())
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
                            a->setName("arg_"+std::to_string(indexGen++));
                        }
                        usedNames.insert(a->name());
                    }
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

}}}}
