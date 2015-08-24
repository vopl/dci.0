#include <dci/couple/parser/errorInfo.hpp>
#include "../ast.hpp"
#include <dci/couple/meta/libraryBuilder.hpp>
#include <dci/logger/logger.hpp>
#include <algorithm>

namespace  dci { namespace couple { namespace parser { namespace impl { namespace ast
{
    class MetaLinker
        : public boost::static_visitor<>
    {
        meta::LibraryBuilder &_lb;

    public:
        MetaLinker(meta::LibraryBuilder &lb)
            : _lb(lb)
        {
        }

        void exec(const Scope &s)
        {
            this->operator()(s.get());
        }

    private:
        template <class V>
        void exec(std::vector<V> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const V &v) {boost::apply_visitor(*this, v);}
            );
        }

        void exec(std::vector<VariantField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const VariantField &v) {
                    _lb.setType(v->meta, typeUse2Meta(v->type));
                    boost::apply_visitor(*this, v->type);
                }
            );
        }

        void exec(std::vector<StructField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const StructField &v) {
                    _lb.setType(v->meta, typeUse2Meta(v->type));
                    boost::apply_visitor(*this, v->type);
                }
            );
        }

        void exec(std::vector<EnumField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const EnumField &v) {
                    _lb.setValue(v->meta, v->value);
                }
            );
        }

        void exec(std::vector<ErrcField> &vs)
        {
            (void)vs;
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const ErrcField &v) {
                    _lb.setDescription(v->meta, v->description);
                }
            );
        }

        void exec(std::vector<Method> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const Method &v) {
                    _lb.setMethodDirection(v->meta, MethodDirection::in==v->direction ? meta::CallDirection::in : meta::CallDirection::out);
                    exec(v->query);

                    _lb.setMethodNoreply(v->meta, v->noreply);

                    std::for_each(
                        v->reply.begin(),
                        v->reply.end(),
                        [&](TypeUse &t) {
                            boost::apply_visitor(*this, t);
                            _lb.addReplyArg(v->meta, typeUse2Meta(t));
                        }
                    );
                }
            );
        }

        void exec(std::vector<MethodParam> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const MethodParam &v) {
                    _lb.setType(v->meta, typeUse2Meta(v->type));
                    boost::apply_visitor(*this, v->type);
                }
            );
        }

    public:
        template <class T>
        void operator()(const std::shared_ptr<T> v)
        {
            (*this)(v.get());
        }

        template <class T>
        void operator()(T &v)
        {
            (*this)(&v);
        }

        void operator()(SVariant *v)
        {
            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SStruct *v)
        {
            if(v->bases)
            {
                std::for_each(
                    v->bases->instances.begin(),
                    v->bases->instances.end(),
                    [&](SStruct *b) {
                        _lb.addBase(v->meta, b->meta);
                    }
                );
            }

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SEnum *v)
        {
            exec(v->fields);
        }

        void operator()(SErrc *v)
        {
            exec(v->fields);
        }

        void operator()(SInterface *v)
        {
            if(v->bases)
            {
                std::for_each(
                    v->bases->instances.begin(),
                    v->bases->instances.end(),
                    [&](SInterface *b) {
                        _lb.addBase(v->meta, b->meta);
                    }
                );
            }
            _lb.setInterfacePrimary(v->meta, v->primary);
            _lb.setInterfaceOpposite(v->meta, v->opposite.lock()->meta);

            exec(v->decls);
            exec(v->methods);
        }

        void operator()(SScope *v)
        {
            exec(v->decls);
        }

        struct TypeUseMetaFetcher
            : public boost::static_visitor<meta::Type *>
        {
            template <class T>
            meta::Type *operator()(const T &v) const
            {
                return v->meta;
            }

            struct ScopedNameMetaFetcher
                : public boost::static_visitor<meta::Type *>
            {
                template <class T>
                meta::Type *operator()(const T &v) const
                {
                    return v->meta;
                }
            };

            meta::Type *operator()(const ScopedName &v) const
            {
                return boost::apply_visitor(ScopedNameMetaFetcher(), v->asDecl);
            }
        };

        meta::Type *typeUse2Meta(TypeUse &tu)
        {
            return boost::apply_visitor(TypeUseMetaFetcher(), tu);
        }

        void operator()(SAlias *v)
        {
            boost::apply_visitor(*this, v->type);
            _lb.setType(v->meta, typeUse2Meta(v->type));
        }

        void operator()(SList *v)
        {
            boost::apply_visitor(*this, v->elementType);
            _lb.setElementType(v->meta, typeUse2Meta(v->elementType));
        }

        void operator()(SSet *v)
        {
            boost::apply_visitor(*this, v->elementType);
            _lb.setElementType(v->meta, typeUse2Meta(v->elementType));
        }

        void operator()(SMap *v)
        {
            boost::apply_visitor(*this, v->keyType);
            boost::apply_visitor(*this, v->valueType);
            _lb.setElementType(v->meta, typeUse2Meta(v->keyType), typeUse2Meta(v->valueType));
        }

        void operator()(SPtr *v)
        {
            boost::apply_visitor(*this, v->valueType);
            _lb.setElementType(v->meta, typeUse2Meta(v->valueType));
        }

        void operator()(SArray *v)
        {
            boost::apply_visitor(*this, v->elementType);
            _lb.setElementType(v->meta, typeUse2Meta(v->elementType));

            //TODO
            try
            {
                _lb.setArraySize(v->meta, static_cast<std::uint32_t>(std::stoull(v->size)));
            }
            catch(const std::runtime_error &e)
            {
                LOGE(e.what());
                _lb.setArraySize(v->meta, 0);
            }
        }

        void operator()(SPrimitive *v)
        {
            _lb.setPrimitiveKind(v->meta, v->kind);
        }

        void operator()(SScopedName *v)
        {
            (void)v;
        }

    };
}}}}}
