#pragma once

#include <memory>
#include <boost/iterator/iterator_adaptor.hpp>

namespace dci { namespace couple { namespace parser { namespace impl
{
    template <class Iterator>
    class PosIterator
        : public boost::iterator_adaptor<
              PosIterator<Iterator>        // Derived
            , Iterator                     // Base
            , boost::use_default           // Value
            , boost::forward_traversal_tag // CategoryOrTraversal
        >
    {
    public:
        PosIterator();

        explicit PosIterator(Iterator, const std::string &file);

        operator bool() const;
        bool operator!() const;

        const std::string &file() const;
        int line() const;
        int column() const;

    private:
        friend class boost::iterator_core_access;

        void increment();

        std::shared_ptr<std::string> _filePtr;
        int _line;
        int _column;
        typename std::iterator_traits<Iterator>::value_type _prev;
    };
    //]

    template <class Iterator>
    PosIterator<Iterator>::PosIterator()
        : PosIterator::iterator_adaptor()
        , _filePtr()
        , _line(-1)
        , _column(-1)
        , _prev(0)
    { }

    template <class Iterator>
    PosIterator<Iterator>::PosIterator(Iterator base, const std::string &file)
        : PosIterator::iterator_adaptor(base)
        , _filePtr(new std::string(file))
        , _line(1)
        , _column(1)
        , _prev(0)
    { }

    template <class Iterator>
    PosIterator<Iterator>::operator bool() const
    {
        return static_cast<bool>(_filePtr);
    }

    template <class Iterator>
    bool PosIterator<Iterator>::operator!() const
    {
        return !operator bool();
    }

    template <class Iterator>
    const std::string &PosIterator<Iterator>::file() const
    {
        static std::string staticStub = "unknown file";
        return _filePtr ? *_filePtr : staticStub;
    }

    template <class Iterator>
    int PosIterator<Iterator>::line() const
    {
        return _line;
    }

    template <class Iterator>
    int PosIterator<Iterator>::column() const
    {
        return _column;
    }

    template<class Iterator>
    void PosIterator<Iterator>::increment()
    {
        typename std::iterator_traits<Iterator>::reference ref = *(this->base());
      
        switch(ref)
        {
        case '\r':
            if(_prev != '\n')
            {
                ++_line;
                _column = 1;
            }
            break;
        case '\n':
            if(_prev != '\r')
            {
                ++_line;
                _column = 1;
            }
            break;
        default:
            ++_column;
            break;
        }
      
        _prev = ref;
        ++this->base_reference();
    }

}}}}
