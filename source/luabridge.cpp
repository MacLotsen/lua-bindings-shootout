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

// LuaBridge expects that we include this ourselves
#include <lua.hpp>

#include <LuaBridge/LuaBridge.h>

#include "benchmark.hpp"
#include "lbs_lua.hpp"
#include "lbs_lib.hpp"

#include <memory>

void luabridge_global_string_get_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	std::string precode = "value = 3";
	lbs::lua_bench_do_or_die(L, precode);

	double x = 0;
	for (auto _ : benchmark_state) {
		double v = luabridge::getGlobal(L, "value");
		x += v;
	}
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
}

void luabridge_global_string_set_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	lbs::lua_bench_do_or_die(L, "value = 3");
	double v = 0;
	for (auto _ : benchmark_state) {
		v += 3;
		luabridge::setGlobal(L, v, "value");
	}
	double x = luabridge::getGlobal(L, "value");
	lbs::expect(benchmark_state, x, v);
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
	lbs::expect(benchmark_state, v, benchmark_state.iterations() * 3);
}

void luabridge_table_chained_get_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	lbs::lua_bench_do_or_die(L, "ulahibe = {warble = {value = 3}}");
	double x = 0;
	for (auto _ : benchmark_state) {
		luabridge::LuaRef tw = luabridge::getGlobal(L, "ulahibe")["warble"];
		double v = tw["value"];
		x += v;
	}
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
}

void luabridge_table_chained_set_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	lbs::lua_bench_do_or_die(L, "ulahibe = {warble = {value = 3}}");
	double v = 0;
	for (auto _ : benchmark_state) {
		v += 3;
		luabridge::LuaRef tw = luabridge::getGlobal(L, "ulahibe")["warble"];
		tw["value"] = v;
	}
	luabridge::LuaRef tw = luabridge::getGlobal(L, "ulahibe")["warble"];
	double x = tw["value"];
	lbs::expect(benchmark_state, x, v);
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
	lbs::expect(benchmark_state, v, benchmark_state.iterations() * 3);
}

void luabridge_table_get_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	lbs::lua_bench_do_or_die(L, "warble = {value = 3}");
	luabridge::LuaRef t = luabridge::getGlobal(L, "warble");
	double x = 0;
	for (auto _ : benchmark_state) {
		double v = t["value"];
		x += v;
	}
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
}

void luabridge_table_set_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	lbs::lua_bench_do_or_die(L, "warble = {value = 3}");
	luabridge::LuaRef t = luabridge::getGlobal(L, "warble");
	double v = 0;
	for (auto _ : benchmark_state) {
		v += 3;
		t["value"] = v;
	}
	double x = luabridge::getGlobal(L, "warble")["value"];
	lbs::expect(benchmark_state, x, v);
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
	lbs::expect(benchmark_state, v, benchmark_state.iterations() * 3);
}

void luabridge_c_function_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.addFunction("f", lbs::basic_call);

	std::string code = lbs::repeated_code(lbs::c_function_code);

	int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
	for (auto _ : benchmark_state) {
		lbs::lua_bench_preload_do_or_die(L, code_index);
	}
	lbs::lua_bench_unload(L, code_index);
}

void luabridge_lua_function_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	lbs::lua_bench_do_or_die(L, "function f(i) return i end");

	luabridge::LuaRef f = luabridge::getGlobal(L, "f");
	double x = 0;
	for (auto _ : benchmark_state) {
		double v = f(3);
		x += v;
	}
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
}

void luabridge_c_through_lua_function_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.addFunction("f", lbs::basic_call);

	luabridge::LuaRef f = luabridge::getGlobal(L, "f");
	double x = 0;
	for (auto _ : benchmark_state) {
		double v = f(3);
		x += v;
	}
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 3);
}

void luabridge_member_function_call_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::basic>("c")
		.addConstructor<lbs::basic (*)()>()
		.addFunction("set", &lbs::basic::set)
		.addFunction("get", &lbs::basic::get)
		.endClass();

	lbs::lua_bench_do_or_die(L, "b = c()");

	lbs::lua_bench_do_or_die(L, lbs::member_function_call_check);

	auto code = lbs::repeated_code(lbs::member_function_call_code);
	int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
	for (auto _ : benchmark_state) {
		lbs::lua_bench_preload_do_or_die(L, code_index);
	}
	lbs::lua_bench_unload(L, code_index);
}

void luabridge_userdata_variable_access_measure(benchmark::State& benchmark_state) {
	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::basic>("c")
		.addConstructor<lbs::basic (*)()>()
		.addData("var", &lbs::basic::var)
		.addData("var0", &lbs::basic::var0)
		.addData("var1", &lbs::basic::var1)
		.addData("var2", &lbs::basic::var2)
		.addData("var3", &lbs::basic::var3)
		.addData("var4", &lbs::basic::var4)
		.endClass();

	lbs::lua_bench_do_or_die(L, "b = c()");

	lbs::lua_bench_do_or_die(L, lbs::userdata_variable_access_check);

	std::string code = lbs::repeated_code(lbs::userdata_variable_access_code);
	int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
	for (auto _ : benchmark_state) {
		lbs::lua_bench_preload_do_or_die(L, code_index);
	}
	lbs::lua_bench_unload(L, code_index);
}

void luabridge_userdata_variable_access_large_measure(benchmark::State& benchmark_state) {
	// Unsupported
	// LuaBridge doesn't understand 64-bit integers... kek
	lbs::unsupported(benchmark_state);
	return;

	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::basic_large>("c")
		.addConstructor<lbs::basic_large (*)()>()
		.addData("var", &lbs::basic_large::var)
		.addData("var0", &lbs::basic_large::var0)
		.addData("var1", &lbs::basic_large::var1)
		.addData("var2", &lbs::basic_large::var2)
		.addData("var3", &lbs::basic_large::var3)
		.addData("var4", &lbs::basic_large::var4)
		.addData("var5", &lbs::basic_large::var5)
		.addData("var6", &lbs::basic_large::var6)
		.addData("var7", &lbs::basic_large::var7)
		.addData("var8", &lbs::basic_large::var8)
		.addData("var9", &lbs::basic_large::var9)
		.addData("var10", &lbs::basic_large::var10)
		.addData("var11", &lbs::basic_large::var11)
		.addData("var12", &lbs::basic_large::var12)
		.addData("var13", &lbs::basic_large::var13)
		.addData("var14", &lbs::basic_large::var14)
		.addData("var15", &lbs::basic_large::var15)
		.addData("var16", &lbs::basic_large::var16)
		.addData("var17", &lbs::basic_large::var17)
		.addData("var18", &lbs::basic_large::var18)
		.addData("var19", &lbs::basic_large::var19)
		.addData("var20", &lbs::basic_large::var20)
		.addData("var21", &lbs::basic_large::var21)
		.addData("var22", &lbs::basic_large::var22)
		.addData("var23", &lbs::basic_large::var23)
		.addData("var24", &lbs::basic_large::var24)
		.addData("var25", &lbs::basic_large::var25)
		.addData("var26", &lbs::basic_large::var26)
		.addData("var27", &lbs::basic_large::var27)
		.addData("var28", &lbs::basic_large::var28)
		.addData("var29", &lbs::basic_large::var29)
		.addData("var30", &lbs::basic_large::var30)
		.addData("var31", &lbs::basic_large::var31)
		.addData("var32", &lbs::basic_large::var32)
		.addData("var33", &lbs::basic_large::var33)
		.addData("var34", &lbs::basic_large::var34)
		.addData("var35", &lbs::basic_large::var35)
		.addData("var36", &lbs::basic_large::var36)
		.addData("var37", &lbs::basic_large::var37)
		.addData("var38", &lbs::basic_large::var38)
		.addData("var39", &lbs::basic_large::var39)
		.addData("var40", &lbs::basic_large::var40)
		.addData("var41", &lbs::basic_large::var41)
		.addData("var42", &lbs::basic_large::var42)
		.addData("var43", &lbs::basic_large::var43)
		.addData("var44", &lbs::basic_large::var44)
		.addData("var45", &lbs::basic_large::var45)
		.addData("var46", &lbs::basic_large::var46)
		.addData("var47", &lbs::basic_large::var47)
		.addData("var48", &lbs::basic_large::var48)
		.addData("var49", &lbs::basic_large::var49)
		.endClass();

	lbs::lua_bench_do_or_die(L, "b = c()");

	lbs::lua_bench_do_or_die(L, lbs::userdata_variable_access_large_check);

	std::string code = lbs::repeated_code(lbs::userdata_variable_access_large_code);
	int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
	for (auto _ : benchmark_state) {
		lbs::lua_bench_preload_do_or_die(L, code_index);
	}
	lbs::lua_bench_unload(L, code_index);
}

void luabridge_userdata_variable_access_last_measure(benchmark::State& benchmark_state) {
	// Unsupported
	// LuaBridge doesn't understand 64-bit integers... kek
	lbs::unsupported(benchmark_state);
	return;

	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::basic_large>("c")
		.addConstructor<lbs::basic_large (*)()>()
		.addData("var", &lbs::basic_large::var)
		.addData("var0", &lbs::basic_large::var0)
		.addData("var1", &lbs::basic_large::var1)
		.addData("var2", &lbs::basic_large::var2)
		.addData("var3", &lbs::basic_large::var3)
		.addData("var4", &lbs::basic_large::var4)
		.addData("var5", &lbs::basic_large::var5)
		.addData("var6", &lbs::basic_large::var6)
		.addData("var7", &lbs::basic_large::var7)
		.addData("var8", &lbs::basic_large::var8)
		.addData("var9", &lbs::basic_large::var9)
		.addData("var10", &lbs::basic_large::var10)
		.addData("var11", &lbs::basic_large::var11)
		.addData("var12", &lbs::basic_large::var12)
		.addData("var13", &lbs::basic_large::var13)
		.addData("var14", &lbs::basic_large::var14)
		.addData("var15", &lbs::basic_large::var15)
		.addData("var16", &lbs::basic_large::var16)
		.addData("var17", &lbs::basic_large::var17)
		.addData("var18", &lbs::basic_large::var18)
		.addData("var19", &lbs::basic_large::var19)
		.addData("var20", &lbs::basic_large::var20)
		.addData("var21", &lbs::basic_large::var21)
		.addData("var22", &lbs::basic_large::var22)
		.addData("var23", &lbs::basic_large::var23)
		.addData("var24", &lbs::basic_large::var24)
		.addData("var25", &lbs::basic_large::var25)
		.addData("var26", &lbs::basic_large::var26)
		.addData("var27", &lbs::basic_large::var27)
		.addData("var28", &lbs::basic_large::var28)
		.addData("var29", &lbs::basic_large::var29)
		.addData("var30", &lbs::basic_large::var30)
		.addData("var31", &lbs::basic_large::var31)
		.addData("var32", &lbs::basic_large::var32)
		.addData("var33", &lbs::basic_large::var33)
		.addData("var34", &lbs::basic_large::var34)
		.addData("var35", &lbs::basic_large::var35)
		.addData("var36", &lbs::basic_large::var36)
		.addData("var37", &lbs::basic_large::var37)
		.addData("var38", &lbs::basic_large::var38)
		.addData("var39", &lbs::basic_large::var39)
		.addData("var40", &lbs::basic_large::var40)
		.addData("var41", &lbs::basic_large::var41)
		.addData("var42", &lbs::basic_large::var42)
		.addData("var43", &lbs::basic_large::var43)
		.addData("var44", &lbs::basic_large::var44)
		.addData("var45", &lbs::basic_large::var45)
		.addData("var46", &lbs::basic_large::var46)
		.addData("var47", &lbs::basic_large::var47)
		.addData("var48", &lbs::basic_large::var48)
		.addData("var49", &lbs::basic_large::var49)
		.endClass();

	lbs::lua_bench_do_or_die(L, "b = c()");

	lbs::lua_bench_do_or_die(L, lbs::userdata_variable_access_large_last_check);

	auto code = lbs::repeated_code(lbs::userdata_variable_access_large_last_code);
	int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
	for (auto _ : benchmark_state) {
		lbs::lua_bench_preload_do_or_die(L, code_index);
	}
	lbs::lua_bench_unload(L, code_index);
}

void luabridge_stateful_function_object_measure(benchmark::State& benchmark_state) {
	// Unsupported
	// You need to manually craft the metatable and set it on the object
	// in order to have specific metamethods behave properly
	lbs::unsupported(benchmark_state);
	return;

	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::basic_stateful>("c")
		.endClass();
	luabridge::LuaRef f = luabridge::getGlobal(L, "f");
	double x = 0;
	for (auto _ : benchmark_state) {
		double v = f(3);
		x += v;
	}
}

void luabridge_multi_return_measure(benchmark::State& benchmark_state) {
	// doesn't understand tuples or out-parameters
	lbs::unsupported(benchmark_state);
	return;

	/*
	auto lua = lbs::create_state();
	lua_State* L = lua.get();

	luabridge::setGlobal(L, &lbs::basic_multi_return, "f");
	luabridge::LuaRef f = luabridge::getGlobal(L, "f");
	double x = 0;
	for (auto _ : benchmark_state) {
		std::tuple<double, double> v = f(3);
		x += std::get<0>(v);
		x += std::get<1>(v);
	}
	lbs::expect(benchmark_state, x, benchmark_state.iterations() * 9);
	*/
}

void luabridge_base_derived_measure(benchmark::State& benchmark_state) {
	// Unsupported.
	// It seems like luabridge has no facilities for base casting
	// from a derived stored in Lua
	// It also only supports single inheritance
	lbs::unsupported(benchmark_state);
	return;
}

void luabridge_return_userdata_measure(benchmark::State& benchmark_state) {
	// CAVEAT: you must register
	// the class
	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::basic>("c")
		.endClass()
		.addFunction("f", &lbs::basic_return)
		.addFunction("h", &lbs::basic_get);

	lbs::lua_bench_do_or_die(L, lbs::return_userdata_check);

	auto code = lbs::repeated_code(lbs::return_userdata_code);
	int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
	for (auto _ : benchmark_state) {
		lbs::lua_bench_preload_do_or_die(L, code_index);
	}
	lbs::lua_bench_unload(L, code_index);
}

void luabridge_optional_measure(benchmark::State& benchmark_state) {
	// no way to ensure optional table digging without
	// the C API
	lbs::unsupported(benchmark_state);
	return;
}

void luabridge_implicit_inheritance_measure(benchmark::State& benchmark_state) {
	// Unsupported?
	// Ask author
	auto lua = lbs::create_state(true);
	lua_State* L = lua.get();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::complex_base_a>("ca")
		.addConstructor<lbs::complex_base_a (*)()>()
		.addFunction("a_func", &lbs::complex_base_a::a_func)
		.addData("a", &lbs::complex_base_a::a)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.beginClass<lbs::complex_base_b>("cb")
		.addConstructor<lbs::complex_base_b (*)()>()
		.addFunction("b_func", &lbs::complex_base_b::b_func)
		.addData("b", &lbs::complex_base_b::b)
		.endClass();

	luabridge::getGlobalNamespace(L)
		.deriveClass<lbs::complex_ab, lbs::complex_base_b>("cab")
		.addConstructor<lbs::complex_ab (*)()>()
		.addFunction("ab_func", &lbs::complex_ab::ab_func)
		.addData("ab", &lbs::complex_ab::ab)
		.endClass();

	lbs::lua_bench_do_or_die(L, "b = cab()");

	{
		lbs::lua_bench_do_or_die(L, "a = b:b_func()");
		double value = luabridge::getGlobal(L, "a");
		if (value != 3) {
			lbs::unsupported(benchmark_state);
			return;
		}
	}

	lbs::lua_bench_do_or_die(L, lbs::implicit_inheritance_check);

	std::string code = lbs::repeated_code(lbs::implicit_inheritance_code);
	int code_index = lbs::lua_bench_load_up(L, code.c_str(), code.size());
	for (auto _ : benchmark_state) {
		lbs::lua_bench_preload_do_or_die(L, code_index);
	}
	lbs::lua_bench_unload(L, code_index);
}

BENCHMARK(luabridge_global_string_get_measure);
BENCHMARK(luabridge_global_string_set_measure);
BENCHMARK(luabridge_table_get_measure);
BENCHMARK(luabridge_table_set_measure);
BENCHMARK(luabridge_table_chained_get_measure);
BENCHMARK(luabridge_table_chained_set_measure);
BENCHMARK(luabridge_c_function_measure);
BENCHMARK(luabridge_c_through_lua_function_measure);
BENCHMARK(luabridge_lua_function_measure);
BENCHMARK(luabridge_member_function_call_measure);
BENCHMARK(luabridge_userdata_variable_access_measure);
BENCHMARK(luabridge_userdata_variable_access_large_measure);
BENCHMARK(luabridge_userdata_variable_access_last_measure);
BENCHMARK(luabridge_multi_return_measure);
BENCHMARK(luabridge_stateful_function_object_measure);
BENCHMARK(luabridge_base_derived_measure);
BENCHMARK(luabridge_return_userdata_measure);
BENCHMARK(luabridge_optional_measure);
BENCHMARK(luabridge_implicit_inheritance_measure);
