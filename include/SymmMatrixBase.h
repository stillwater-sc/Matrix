// Hossein Moein
// August 1, 2009
// Copyright (C) 2017-2018 Hossein Moein
// Distributed under the BSD Software License (see file License)

#ifndef _INCLUDED_SymmMatrixBase_h
#define _INCLUDED_SymmMatrixBase_h

#include <iostream>

#include <DMScu_VectorRange.h>
#include <DMScu_StepVectorRange.h>

#include <MatrixBase.h>

// ----------------------------------------------------------------------------

namespace hmma
{

template<class TYPE>
class   SymmMatrixBase : public DenseMatrixStorage<TYPE>  {

    public:

        typedef DenseMatrixStorage<TYPE> BaseClass;

        typedef typename BaseClass::size_type       size_type;
        typedef typename BaseClass::value_type      value_type;
        typedef typename BaseClass::reference       reference;
        typedef typename BaseClass::const_reference const_reference;
        typedef typename BaseClass::pointer         pointer;
        typedef typename BaseClass::const_pointer   const_pointer;

        typedef typename BaseClass::Exception       Exception;
        typedef typename BaseClass::NotSquare       NotSquare;
        typedef typename BaseClass::Singular        Singular;
        typedef typename BaseClass::NotSolvable     NotSolvable;

        typedef SymmMatrixBase<value_type>   SelfType;

    protected:

        inline SymmMatrixBase () throw ()  {   }

        inline
        SymmMatrixBase (size_type row,
                               size_type col,
                               const_reference def_value = value_type ())
            throw (NotSquare)
            : BaseClass (row, col, (col * (col + 1)) / 2, def_value)  {

            if (row != col)
                throw NotSquare ();
        }

        static inline bool _is_symmetric_matrix () throw()  { return (true); }

    public:

        void resize (size_type in_row,
                     size_type in_col, 
                     const_reference def_value = value_type ());

        inline reference at (size_type r, size_type c) throw ();
        inline const_reference at (size_type r, size_type c) const throw ();

        std::ostream &dump (std::ostream &out_stream) const;

    public:

        class   iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the iterator to be an "undefined" iterator
               //
                inline iterator () throw () : matx_ (NULL), idx_ (0)  {   }

                inline iterator (SelfType *m, size_type idx = 0) throw ()
                    : matx_ (m), idx_ (idx)  {   }

                inline bool operator == (const iterator &rhs) const  {

                    return (matx_ == rhs.matx_ && idx_ == rhs.idx_);
                }
                inline bool operator != (const iterator &rhs) const  {

                    return (matx_ != rhs.matx_ || idx_ != rhs.idx_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline pointer operator -> () const throw ()  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }
                inline reference operator * () const throw ()  {

                    return (matx_->at (idx_ / matx_->columns (),
                                       idx_ % matx_->columns ()));
                }
                inline operator pointer () const throw ()  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }

               // We are following STL style iterator interface.
               //
                inline iterator &operator ++ () throw ()  {    // ++Prefix

                    idx_ += 1;
                    return (*this);
                }
                inline iterator operator ++ (int) throw ()  {  // Postfix++

                    const   size_type   ret_idx = idx_;

                    idx_ += 1;
                    return (iterator (matx_, ret_idx));
                }

                inline iterator &operator += (long i) throw ()  {

                    idx_ += i;
                    return (*this);
                }

                inline iterator &operator -- () throw ()  {    // --Prefix

                    idx_ -= 1;
                    return (*this);
                }
                inline iterator operator -- (int) throw ()  {  // Postfix--

                    const   size_type   ret_idx = idx_;

                    idx_ -= 1;
                    return (iterator (matx_, ret_idx));
                }

                inline iterator &operator -= (int i) throw ()  {

                    idx_ -= i;
                    return (*this);
                }

                inline iterator operator + (int i) throw ()  {

                    return (iterator (matx_, idx_ + i));
                }

                inline iterator operator - (int i) throw ()  {

                    return (iterator (matx_, idx_ - i));
                }

                inline iterator operator + (long i) throw ()  {

                    return (iterator (matx_, idx_ + i));
                }

                inline iterator operator - (long i) throw ()  {

                    return (iterator (matx_, idx_ - i));
                }

            private:

                SelfType    *matx_;
                size_type   idx_;

                friend  class   SymmMatrixBase::const_iterator;
        };

        class   const_iterator
            : public std::iterator<std::random_access_iterator_tag,
                                   value_type, long>  {

            public:

                typedef std::random_access_iterator_tag iterator_category;

            public:

               // NOTE: The constructor with no argument initializes
               //       the const_iterator to be an "undefined"
               //       const_iterator
               //
                inline const_iterator () throw ()
                    : matx_ (NULL), idx_ (0)  {   }

                inline const_iterator (const SelfType *m, 
                                       size_type idx = 0) throw ()
                    : matx_ (m), idx_ (idx)  {   }

                inline const_iterator (
                    const typename SelfType::iterator &that)  {

                    *this = that;
                }

                inline const_iterator &operator = (
                    const typename SelfType::iterator &rhs)  {

                    matx_ = rhs.matx_;
                    idx_ = rhs.idx_;
                    return (*this);
                }

                inline bool operator == (const const_iterator &rhs) const {

                    return (matx_ == rhs.matx_ && idx_ == rhs.idx_);
                }
                inline bool operator != (const const_iterator &rhs) const {

                    return (matx_ != rhs.matx_ || idx_ != rhs.idx_);
                }

               // Following STL style, this iterator appears as a pointer
               // to value_type.
               //
                inline const_pointer operator -> () const throw ()  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }
                inline const_reference operator * () const throw ()  {

                    return (matx_->at (idx_ / matx_->columns (),
                                       idx_ % matx_->columns ()));
                }
                inline operator const_pointer () const throw ()  {

                    return (&(matx_->at (idx_ / matx_->columns (),
                                         idx_ % matx_->columns ())));
                }

               // ++Prefix
               //
                inline const_iterator &operator ++ () throw ()  {

                    idx_ += 1;
                    return (*this);
                }

               // Postfix++
               //
                inline const_iterator operator ++ (int) throw ()  {

                    const   size_type   ret_idx = idx_;

                    idx_ += 1;
                    return (const_iterator (matx_, ret_idx));
                }
                inline const_iterator &operator += (long i) throw ()  {

                    idx_ += i;
                    return (*this);
                }

               // --Prefix
               //
                inline const_iterator &operator -- () throw ()  {

                    idx_ -= 1;
                    return (*this);
                }

               // Postfix--
               //
                inline const_iterator operator -- (int) throw ()  {

                    const   size_type   ret_idx = idx_;

                    idx_ -= 1;
                    return (const_iterator (matx_, ret_idx));
                }

                inline const_iterator &operator -= (int i) throw ()  {

                    idx_ -= i;
                    return (*this);
                }

                inline const_iterator operator + (int i) throw ()  {

                    return (const_iterator (matx_, idx_ + i));
                }

                inline const_iterator operator - (int i) throw ()  {

                    return (const_iterator (matx_, idx_ - i));
                }

                inline const_iterator operator + (long i) throw ()  {

                    return (const_iterator (matx_, idx_ + i));
                }

                inline const_iterator operator - (long i) throw ()  {

                    return (const_iterator (matx_, idx_ - i));
                }

            private:

                const   SelfType    *matx_;
                size_type           idx_;
        };

        typedef iterator        row_iterator;
        typedef iterator        col_iterator;
        typedef const_iterator  row_const_iterator;
        typedef const_iterator  col_const_iterator;

        inline col_iterator col_begin () throw ()  {

            return (col_iterator (this));
        }
        inline col_const_iterator col_begin () const throw ()  {

            return (col_const_iterator (this));
        }
        inline col_iterator col_end () throw ()  {

            return (col_iterator (this,
                                  BaseClass::rows () * BaseClass::columns ()));
        }
        inline col_const_iterator col_end () const throw ()  {

            return (col_const_iterator (
                        this,
                        BaseClass::rows () * BaseClass::columns ()));
        }

        inline row_iterator row_begin () throw ()  {

            return (row_iterator (this));
        }
        inline row_const_iterator row_begin () const throw ()  {

            return (row_const_iterator (this));
        }
        inline row_iterator row_end () throw ()  {

            return (row_iterator (this,
                                  BaseClass::rows () * BaseClass::columns ()));
        }
        inline row_const_iterator row_end () const throw ()  {

            return (row_const_iterator (
                        this,
                        BaseClass::rows () * BaseClass::columns ()));
        }
};

} // namespace hmma

// ----------------------------------------------------------------------------

#  ifdef DMS_INCLUDE_SOURCE
#    include <SymmMatrixBase.tcc>
#  endif // DMS_INCLUDE_SOURCE

// ----------------------------------------------------------------------------

#undef _INCLUDED_SymmMatrixBase_h
#define _INCLUDED_SymmMatrixBase_h 1
#endif  // _INCLUDED_SymmMatrixBase_h

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End: