/**
 * KFR (http://kfrlib.com)
 * Copyright (C) 2016  D Levin
 * See LICENSE.txt for details
 */

#include "testo/testo.hpp"

#include <kfr/base.hpp>
#include <kfr/dsp.hpp>
#include <kfr/io.hpp>

using namespace kfr;

TEST(pack)
{
    const univector<float, 20> v1 = 1 + counter();
    const univector<float, 20> v2 = v1 * 11;
    const univector<f32x2, 20> v3 = pack(v1, v2);
    CHECK(v3[0] == f32x2{ 1, 11 });
    CHECK(v3[1] == f32x2{ 2, 22 });
    CHECK(v3[18] == f32x2{ 19, 209 });
    CHECK(v3[19] == f32x2{ 20, 220 });

    const univector<f32x2, 20> v4 = bind_expression(fn::reverse(), v3);
    CHECK(v4[0] == f32x2{ 11, 1 });
    CHECK(v4[1] == f32x2{ 22, 2 });
    CHECK(v4[18] == f32x2{ 209, 19 });
    CHECK(v4[19] == f32x2{ 220, 20 });
}

TEST(adjacent)
{
    univector<int, 20> v1 = adjacent(fn::mul(), counter());
    CHECK(v1[0] == 0);
    CHECK(v1[1] == 0);
    CHECK(v1[2] == 2);
    CHECK(v1[3] == 6);
    CHECK(v1[4] == 12);
    CHECK(v1[19] == 342);
}

TEST(rebind)
{
    auto c_minus_two  = counter() - 2;
    auto four_minus_c = rebind(c_minus_two, 4, counter());
    univector<int, 5> v1 = c_minus_two;
    univector<int, 5> v2 = four_minus_c;
    CHECK(v1[0] == -2);
    CHECK(v1[1] == -1);
    CHECK(v2[0] == 4);
    CHECK(v2[1] == 3);
}

TEST(test_arg_access)
{
    univector<float> v1(10);
    v1                       = counter();
    auto e1                  = std::move(v1) + 10;
    std::get<0>(e1.args)[0]  = 100;
    std::get<1>(e1.args).val = 1;
    univector<float, 10> v2 = e1;
    CHECK(v2[0] == 101);
    CHECK(v2[1] == 2);
    CHECK(v2[2] == 3);
    CHECK(v2[9] == 10);
}

TEST(test_arg_replace)
{
    univector<float, 10> v1 = counter();
    univector<float, 10> v2 = -counter();
    auto e1              = to_pointer(v1) * 10;
    std::get<0>(e1.args) = to_pointer(v2);
    univector<float, 10> v3 = e1;
    CHECK(v3[0] == 0);
    CHECK(v3[1] == -10);
    CHECK(v3[2] == -20);
    CHECK(v3[9] == -90);
}

TEST(size_calc)
{
    auto a = counter();
    CHECK(a.size() == infinite_size);
    auto b = slice(counter(), 100);
    CHECK(b.size() == infinite_size);
    auto c = slice(counter(), 100, 1000);
    CHECK(c.size() == 1000);
    auto d = slice(c, 100);
    CHECK(d.size() == 900);
}

TEST(reverse)
{
    univector<int, 21> a = reverse(truncate(counter(), 21));
    CHECK(a[0] == 20);
    CHECK(a[1] == 19);
    CHECK(a[20] == 0);
}

int main(int argc, char** argv)
{
    println(library_version());

    return testo::run_all("", true);
}