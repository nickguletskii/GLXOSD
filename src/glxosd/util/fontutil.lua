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

require("ffi/ffitypes")
require("ffi/fontconfig")

local ffi = require("ffi")

return {
	get_font = function (name)
		local fontConfig = fc.FcInitLoadConfigAndFonts();

		local pattern = fc.FcNameParse(ffi.cast(ffi_types.const_FcChar8_ptr, name));

		fc.FcConfigSubstitute(fontConfig, pattern, fc.FcMatchPattern);
		fc.FcDefaultSubstitute(pattern);
		local result = ffi_types.FcResult_ref();
		local fontFile;
		local font = fc.FcFontMatch(fontConfig, pattern, result);

		if (font) then
			local file = ffi_types.FcChar8_ptr_ref();
			if (fc.FcPatternGetString(font, FC_FILE, 0, file) == fc.FcResultMatch) then
				fontFile = ffi.string(file[0]);
			end
			fc.FcPatternDestroy(font);
		end
		fc.FcPatternDestroy(pattern);
		fc.FcConfigDestroy(fontConfig);
		return fontFile;
	end
}
