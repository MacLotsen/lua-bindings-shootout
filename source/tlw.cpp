// lua bindings shootout
// The MIT License (MIT)

// Copyright © 2018 ThePhD

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <tlw/types.hpp>
#include <tlw/tlw.hpp>
#include <tlw/wrapping.hpp>

#include "lbs_lua.hpp"
#include "lbs_lib.hpp"
#include "benchmark.hpp"

static PrettyClassPrototype *basic_large_prototype = PrettyClassPrototypeBuilder("basic_large")
        .property("var", mk_property(&lbs::basic_large::var))
        .property("var0", mk_property(&lbs::basic_large::var0))
        .property("var1", mk_property(&lbs::basic_large::var1))
        .property("var2", mk_property(&lbs::basic_large::var2))
        .property("var3", mk_property(&lbs::basic_large::var3))
        .property("var4", mk_property(&lbs::basic_large::var4))
        .property("var5", mk_property(&lbs::basic_large::var5))
        .property("var6", mk_property(&lbs::basic_large::var6))
        .property("var7", mk_property(&lbs::basic_large::var7))
        .property("var8", mk_property(&lbs::basic_large::var8))
        .property("var9", mk_property(&lbs::basic_large::var9))
        .property("var10", mk_property(&lbs::basic_large::var10))
        .property("var11", mk_property(&lbs::basic_large::var11))
        .property("var12", mk_property(&lbs::basic_large::var12))
        .property("var13", mk_property(&lbs::basic_large::var13))
        .property("var14", mk_property(&lbs::basic_large::var14))
        .property("var15", mk_property(&lbs::basic_large::var15))
        .property("var16", mk_property(&lbs::basic_large::var16))
        .property("var17", mk_property(&lbs::basic_large::var17))
        .property("var18", mk_property(&lbs::basic_large::var18))
        .property("var19", mk_property(&lbs::basic_large::var19))
        .property("var20", mk_property(&lbs::basic_large::var20))
        .property("var21", mk_property(&lbs::basic_large::var21))
        .property("var22", mk_property(&lbs::basic_large::var22))
        .property("var23", mk_property(&lbs::basic_large::var23))
        .property("var24", mk_property(&lbs::basic_large::var24))
        .property("var25", mk_property(&lbs::basic_large::var25))
        .property("var26", mk_property(&lbs::basic_large::var26))
        .property("var27", mk_property(&lbs::basic_large::var27))
        .property("var28", mk_property(&lbs::basic_large::var28))
        .property("var29", mk_property(&lbs::basic_large::var29))
        .property("var30", mk_property(&lbs::basic_large::var30))
        .property("var31", mk_property(&lbs::basic_large::var31))
        .property("var32", mk_property(&lbs::basic_large::var32))
        .property("var33", mk_property(&lbs::basic_large::var33))
        .property("var34", mk_property(&lbs::basic_large::var34))
        .property("var35", mk_property(&lbs::basic_large::var35))
        .property("var36", mk_property(&lbs::basic_large::var36))
        .property("var37", mk_property(&lbs::basic_large::var37))
        .property("var38", mk_property(&lbs::basic_large::var38))
        .property("var39", mk_property(&lbs::basic_large::var39))
        .property("var40", mk_property(&lbs::basic_large::var40))
        .property("var41", mk_property(&lbs::basic_large::var41))
        .property("var42", mk_property(&lbs::basic_large::var42))
        .property("var43", mk_property(&lbs::basic_large::var43))
        .property("var44", mk_property(&lbs::basic_large::var44))
        .property("var45", mk_property(&lbs::basic_large::var45))
        .property("var46", mk_property(&lbs::basic_large::var46))
        .property("var47", mk_property(&lbs::basic_large::var47))
        .property("var48", mk_property(&lbs::basic_large::var48))
        .property("var49", mk_property(&lbs::basic_large::var49))
        .build();

void tlw_table_global_string_get_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    Lua lua(L);

    lua.set("value", lbs::magic_value());
    double x = 0;
    for (auto _ : benchmark_state) {
        auto v = lua.get<double>("value");
        x += v;
    }
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_table_global_string_set_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    Lua lua(L);

    double v = 0;
    for (auto _ : benchmark_state) {
        v += lbs::magic_value();
        lua.set("value", v);
    }
    auto x = lua.get<double>("value");
    lbs::expect(benchmark_state, x, v);
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
    lbs::expect(benchmark_state, v, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_table_get_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    Lua lua(L);

    LuaTable table = lua.table();
    table.set("value", lbs::magic_value());
    lua.set<LuaRef>("warble", table);

    double x = 0;
    for (auto _ : benchmark_state) {
        auto v = table.get<double>("value");
        x += v;
    }
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_table_set_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    Lua lua(L);

    LuaTable table = lua.table();
    table.set<double>("value", lbs::magic_value());

    double v = 0;
    for (auto _ : benchmark_state) {
        v += lbs::magic_value();
        table.set("value", v);
    }
    auto x = table.get<double>("value");
    lbs::expect(benchmark_state, x, v);
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
    lbs::expect(benchmark_state, v, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_table_chained_get_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    Lua lua(L);

    LuaTable table1 = lua.table();
    LuaTable table2 = lua.table();
    table1.set<LuaRef>("warble", table2);
    table2.set("value", lbs::magic_value());
    lua.set<LuaRef>("ulahibe", table1);

    double x = 0;
    for (auto _ : benchmark_state) {
        auto v = lua.get<LuaTable>("ulahibe").get<LuaTable>("warble").get<double>("value");
        x += v;
    }
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_table_chained_set_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    Lua lua(L);

    LuaTable table1 = lua.table();
    LuaTable table2 = lua.table();
    table2.set("value", lbs::magic_value());
    table1.set<LuaRef>("warble", table2);
    lua.set<LuaRef>("ulahibe", table1);
    double v = 0;
    for (auto _ : benchmark_state) {
        v += lbs::magic_value();
        lua.get<LuaTable>("ulahibe").get<LuaTable>("warble").set("value", v);
    }
    auto x = table2.get<double>("value");
    lbs::expect(benchmark_state, x, v);
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
    lbs::expect(benchmark_state, v, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_c_function_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lua.set("f", &lbs::basic_call_wrap);

    lbs::lua_bench_do_or_die(L, lbs::c_function_check);

    std::string code = lbs::repeated_code(lbs::c_function_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

void tlw_lua_function_in_c_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    double x = 0;
    lbs::lua_bench_do_or_die(L, "function f (i) return i end");
    {
        auto f = lua.get<LuaFunction<double(double)>>("f");

        for (auto _ : benchmark_state) {
            double v = f(lbs::magic_value());
            x += v;
        }
    }
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_c_function_through_lua_in_c_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lua.set("f", &lbs::basic_call_wrap);
    double x = 0;
    {
        auto f = lua.get<LuaFunction<double(double)>>("f");

        for (auto _ : benchmark_state) {
            double v = f(lbs::magic_value());
            x += v;
        }
    }
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * lbs::magic_value());
}

void tlw_member_function_call_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lua.add<lbs::basic>(ClassPrototypeBuilder("basic")
                                .method("set", &lbs::basic_set_wrap)
                                .method("get", &lbs::basic_get_wrap)
                                .build());
    lbs::basic b;

    lua.setObject("b", &b);

    lbs::lua_bench_do_or_die(L, lbs::member_function_call_check);

    std::string code = lbs::repeated_code(lbs::member_function_call_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

void tlw_userdata_variable_access_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lua.add<lbs::basic>(PrettyClassPrototypeBuilder("basic")
                                .property("var", mk_property(&lbs::basic::var))
                                .getter("get", &lbs::basic_get_wrap)
                                .setter("set", &lbs::basic_set_wrap)
                                .build());

    lbs::basic b;
    lua.setObject("b", &b);

    lbs::lua_bench_do_or_die(L, lbs::userdata_variable_access_check);

    std::string code = lbs::repeated_code(lbs::userdata_variable_access_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

void tlw_userdata_variable_access_large_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lbs::basic_large b;
    lua.add<lbs::basic_large>(basic_large_prototype).setObject("b", &b);

    lbs::lua_bench_do_or_die(L, lbs::userdata_variable_access_large_check);

    std::string code = lbs::repeated_code(lbs::userdata_variable_access_large_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

void tlw_userdata_variable_access_last_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lbs::basic_large b;
    lua.add<lbs::basic_large>(basic_large_prototype).setObject("b", &b);

    lbs::lua_bench_do_or_die(L, lbs::userdata_variable_access_large_last_check);

    std::string code = lbs::repeated_code(lbs::userdata_variable_access_large_last_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

void tlw_stateful_function_object_measure(benchmark::State &benchmark_state) {
    // No support for functors directly...
    lbs::unsupported(benchmark_state);
}

void tlw_multi_return_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lua.set("f", &lbs::basic_multi_return_wrap);
    double x = 0;
    {
        auto f = lua.get<LuaFunction<std::tuple<double, double>(double)>>("f");

        for (auto _ : benchmark_state) {
            auto r = f(lbs::magic_value());
            double v = std::get<0>(r);
            double w = std::get<1>(r);
            x += v;
            x += w;
        }
    }
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * (lbs::magic_value() * 3));
}

void tlw_multi_return_lua_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lua.set("f", &lbs::basic_multi_return_wrap);

    lbs::lua_bench_do_or_die(L, lbs::lua_multi_return_check);

    std::string code = lbs::repeated_code(lbs::lua_multi_return_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

void tlw_base_derived_measure(benchmark::State &benchmark_state) {
    // Base correctness lost with type B
    lbs::unsupported(benchmark_state);
    return;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    auto cab = ClassPrototypeBuilder("cab")
            .method("a_func", mk_function(&lbs::complex_ab::a_func))
            .method("b_func", mk_function(&lbs::complex_ab::b_func))
            .method("ab_func", mk_function(&lbs::complex_ab::ab_func))
            .build();

    lua.add<lbs::complex_ab>(cab);

    lbs::complex_ab ab;
    // Set and verify correctness
    lua.setObject("b", &ab);
    {
        auto va = lua.get<lbs::complex_base_a*>("b");
        auto vb = lua.get<lbs::complex_base_b*>("b");
        if (!lbs::verify_base_correctness(*va, *vb, ab)) {
            lbs::unsupported(benchmark_state);
            return;
        }
    }
    double x = 0;
    for (auto _ : benchmark_state) {
        auto va = lua.get<lbs::complex_base_a*>("b");
        auto vb = lua.get<lbs::complex_base_b*>("b");
        x += va->a_func();
        x += vb->b_func();
    }
    lbs::expect(benchmark_state, x, benchmark_state.iterations() * (lbs::magic_value() * 2));
}

void tlw_return_userdata_measure(benchmark::State &benchmark_state) {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    lua.set("f", &lbs::basic_return_wrap);
    lua.set("h", &lbs::basic_get_wrap);

    lbs::lua_bench_do_or_die(L, lbs::return_userdata_check);

    std::string code = lbs::repeated_code(lbs::return_userdata_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

void tlw_optional_failure_measure(benchmark::State &benchmark_state) {
    lbs::unsupported(benchmark_state);
}

void tlw_optional_half_failure_measure(benchmark::State &benchmark_state) {
    lbs::unsupported(benchmark_state);
}

void tlw_optional_success_measure(benchmark::State &benchmark_state) {
    lbs::unsupported(benchmark_state);
}

void tlw_implicit_inheritance_measure(benchmark::State &benchmark_state) {
    // It does do the trick for the test, yet now a is extended with b, which is illegal
    // Also extending twice will result in the same behaviour I guess.
    lbs::unsupported(benchmark_state);
    return;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    Lua lua(L);

    auto ca_proto = ClassPrototypeBuilder("ca")
            .method("a_func", mk_function(&lbs::complex_base_a::a_func))
            .build();

    auto cb_proto = ClassPrototypeBuilder("cb")
            .method("b_func", mk_function(&lbs::complex_base_b::b_func))
            .build();

    auto cab_proto = ClassPrototypeBuilder("cab")
            .method("ab_func", mk_function(&lbs::complex_ab::ab_func))
            .build();

    lua.add<lbs::complex_base_a>(ca_proto);
    lua.add<lbs::complex_base_b>(cb_proto);
    lua.add<lbs::complex_ab>(cab_proto);
    lua.extend<lbs::complex_ab, lbs::complex_base_a, lbs::complex_base_b>();

    lbs::complex_ab ab;

    lua.setObject("b", &ab);

    lbs::lua_bench_do_or_die(L, lbs::implicit_inheritance_check);

    std::string code = lbs::repeated_code(lbs::implicit_inheritance_code);
    int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
    for (auto _ : benchmark_state) {
        lbs::lua_bench_preload_do_or_die(L, code_index);
    }
    lbs::lua_bench_unload(L, code_index);
}

BENCHMARK(tlw_table_global_string_get_measure);
BENCHMARK(tlw_table_global_string_set_measure);
BENCHMARK(tlw_table_get_measure);
BENCHMARK(tlw_table_set_measure);
BENCHMARK(tlw_table_chained_get_measure);
BENCHMARK(tlw_table_chained_set_measure);
BENCHMARK(tlw_c_function_measure);
BENCHMARK(tlw_c_function_through_lua_in_c_measure);
BENCHMARK(tlw_lua_function_in_c_measure);
BENCHMARK(tlw_member_function_call_measure);
BENCHMARK(tlw_userdata_variable_access_measure);
BENCHMARK(tlw_userdata_variable_access_large_measure);
BENCHMARK(tlw_userdata_variable_access_last_measure);
BENCHMARK(tlw_multi_return_lua_measure);
BENCHMARK(tlw_multi_return_measure);
BENCHMARK(tlw_stateful_function_object_measure);
BENCHMARK(tlw_base_derived_measure);
BENCHMARK(tlw_return_userdata_measure);
BENCHMARK(tlw_optional_failure_measure);
BENCHMARK(tlw_optional_half_failure_measure);
BENCHMARK(tlw_optional_success_measure);
BENCHMARK(tlw_implicit_inheritance_measure);
