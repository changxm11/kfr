/** @addtogroup math
 *  @{
 */
/*
  Copyright (C) 2016 D Levin (https://www.kfrlib.com)
  This file is part of KFR

  KFR is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KFR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KFR.

  If GPL is not suitable for your project, you must purchase a commercial license to use KFR.
  Buying a commercial license is mandatory as soon as you develop commercial activities without
  disclosing the source code of your own applications.
  See https://www.kfrlib.com for details.
 */
#pragma once

#include "basic_expressions.hpp"
#include "pointer.hpp"
#include "univector.hpp"

namespace kfr
{

template <typename T>
class filter
{
public:
    virtual ~filter() {}
    virtual void reset() {}

    template <size_t Size>
    void apply(T (&buffer)[Size])
    {
        process_buffer(buffer, buffer, Size);
    }

    template <size_t Size>
    void apply(T (&dest)[Size], T (&src)[Size])
    {
        process_buffer(dest, src, Size);
    }

    template <size_t Tag>
    void apply(univector<T, Tag>& buffer)
    {
        process_buffer(buffer.data(), buffer.data(), buffer.size());
    }

    template <size_t Tag1, size_t Tag2>
    void apply(univector<T, Tag1>& dest, const univector<T, Tag2>& src)
    {
        process_buffer(dest.data(), src.data(), std::min(dest.size(), src.size()));
    }

    void apply(T* buffer, size_t size) { process_buffer(buffer, buffer, size); }

    void apply(T* dest, const T* src, size_t size) { process_buffer(dest, src, size); }

    template <size_t Tag>
    void apply(univector<T, Tag>& dest, const expression_pointer<T>& src)
    {
        process_expression(dest.data(), src, size_min(dest.size(), src.size()));
    }

    void apply(T* dest, const expression_pointer<T>& src, size_t size)
    {
        process_expression(dest, src, size_min(size, src.size()));
    }

    template <size_t Tag, typename Expr, KFR_ENABLE_IF(is_input_expression<Expr>::value)>
    void apply(univector<T, Tag>& dest, const Expr& src)
    {
        process_expression(dest.data(), to_pointer(src), size_min(dest.size(), src.size()));
    }

    template <typename Expr, KFR_ENABLE_IF(is_input_expression<Expr>::value)>
    void apply(T* dest, const Expr& src, size_t size)
    {
        process_expression(dest, to_pointer(src), size_min(size, src.size()));
    }

protected:
    virtual void process_buffer(T* dest, const T* src, size_t size)                         = 0;
    virtual void process_expression(T* dest, const expression_pointer<T>& src, size_t size) = 0;
};
}
