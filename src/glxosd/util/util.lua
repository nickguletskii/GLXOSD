--[[
Copyright (C) 2016 Nick Guletskii

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
]]

function log_error(str)
	io.stderr:write(str.."\n")
	io.stderr:write(debug.traceback().."\n")
end

function table.insert_all(tbl, ...)
	for i=1,select('#',...) do
		local add = select(i,...)
		table.insert(tbl, add)
	end
end

function check_class(tbl, name)
	assert(type(tbl) == "table", "A "..name.." was required, but got a "..type(tbl))
	assert(tbl.__class_name, "The provided table is not a "..name)
	assert(tbl.__class_name==name, "The provided table is a ".. tbl.__class_name.. ", not a "..name)
end

function check_key_combo(key_combo, key, modifiers)
	if not key_combo then
		return false
	end
	local modifier_set={}
	for _, modifier in pairs(key_combo.modifiers) do
		modifier_set[modifier] = true
		if not modifiers[modifier] then
			return false
		end
	end
	for modifier, val in pairs(modifiers) do
		if val and not modifier_set[modifier] then
			return false
		end
	end
	return key_combo.main_key == key
end

only_one_enough_voter_metatable = {
	__call = function(self)
		for _, v in pairs(self) do
			if v then
				return true
			end
		end
		return false
	end
}

remove_nils_from_array = function(arr)
	local tmp = {}
	for i=1,table.getn(arr) do
		if arr[i] then
			table.insert(tmp, arr[i])
		end
	end
	return tmp
end
