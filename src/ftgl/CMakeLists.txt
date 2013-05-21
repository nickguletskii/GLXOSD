#Compile the 32 bit version
SET(FTGL_VERSION_SERIES 2)
SET(FTGL_VERSION_MAJOR 1)
SET(FTGL_VERSION_MINOR 3)
SET(FTGL_SOVERSION 1)
ADD_DEFINITIONS(-DPACKAGE_VERSION="${FTGL_VERSION_SERIES}.${FTGL_VERSION_MAJOR}.${FTGL_VERSION_MINOR}")

SET(libftgl_la_SOURCES
    FTBuffer.cpp
    FTCharmap.cpp
    FTCharmap.h
    FTCharToGlyphIndexMap.h
    FTCleanup.cpp
    FTCleanup.h
    FTContour.cpp
    FTContour.h
    FTFace.cpp
    FTFace.h
    FTGL.cpp
    FTGlyphContainer.cpp
    FTGlyphContainer.h
    FTInternals.h
    FTLibrary.cpp
    FTLibrary.h
    FTList.h
    FTPoint.cpp
    FTSize.cpp
    FTSize.h
    FTVector.h
    FTVectoriser.cpp
    FTVectoriser.h
    FTUnicode.h
)

SET(ftgl_headers
    FTGL/ftgl.h
    FTGL/FTBBox.h
    FTGL/FTBuffer.h
    FTGL/FTPoint.h
    FTGL/FTGlyph.h
    FTGL/FTBitmapGlyph.h
    FTGL/FTBufferGlyph.h
    FTGL/FTExtrdGlyph.h
    FTGL/FTOutlineGlyph.h
    FTGL/FTPixmapGlyph.h
    FTGL/FTPolyGlyph.h
    FTGL/FTTextureGlyph.h
    FTGL/FTFont.h
    FTGL/FTGLBitmapFont.h
    FTGL/FTBufferFont.h
    FTGL/FTGLExtrdFont.h
    FTGL/FTGLOutlineFont.h
    FTGL/FTGLPixmapFont.h
    FTGL/FTGLPolygonFont.h
    FTGL/FTGLTextureFont.h
    FTGL/FTGLTriangleExtractorFont.h
    FTGL/FTLayout.h
    FTGL/FTSimpleLayout.h
)

SET(ftglyph_sources
    FTGlyph/FTGlyph.cpp
    FTGlyph/FTGlyphImpl.h
    FTGlyph/FTGlyphGlue.cpp
    FTGlyph/FTBitmapGlyph.cpp
    FTGlyph/FTBitmapGlyphImpl.h
    FTGlyph/FTBufferGlyph.cpp
    FTGlyph/FTBufferGlyphImpl.h
    FTGlyph/FTExtrudeGlyph.cpp
    FTGlyph/FTExtrudeGlyphImpl.h
    FTGlyph/FTOutlineGlyph.cpp
    FTGlyph/FTOutlineGlyphImpl.h
    FTGlyph/FTPixmapGlyph.cpp
    FTGlyph/FTPixmapGlyphImpl.h
    FTGlyph/FTPolygonGlyph.cpp
    FTGlyph/FTPolygonGlyphImpl.h
    FTGlyph/FTTextureGlyph.cpp
    FTGlyph/FTTextureGlyphImpl.h
    FTGlyph/FTTriangleExtractorGlyph.cpp
    FTGlyph/FTTriangleExtractorGlyphImpl.h
)

SET(ftfont_sources
    FTFont/FTFont.cpp
    FTFont/FTFontImpl.h
    FTFont/FTFontGlue.cpp
    FTFont/FTBitmapFont.cpp
    FTFont/FTBitmapFontImpl.h
    FTFont/FTBufferFont.cpp
    FTFont/FTBufferFontImpl.h
    FTFont/FTExtrudeFont.cpp
    FTFont/FTExtrudeFontImpl.h
    FTFont/FTOutlineFont.cpp
    FTFont/FTOutlineFontImpl.h
    FTFont/FTPixmapFont.cpp
    FTFont/FTPixmapFontImpl.h
    FTFont/FTPolygonFont.cpp
    FTFont/FTPolygonFontImpl.h
    FTFont/FTTextureFont.cpp
    FTFont/FTTextureFontImpl.h
    FTFont/FTTriangleExtractorFont.cpp
    FTFont/FTTriangleExtractorFontImpl.h
)

SET(ftlayout_sources
    FTLayout/FTLayout.cpp
    FTLayout/FTLayoutImpl.h
    FTLayout/FTLayoutGlue.cpp
    FTLayout/FTSimpleLayout.cpp
    FTLayout/FTSimpleLayoutImpl.h
)
add_library(ftgl SHARED ${libftgl_la_SOURCES} ${ftgl_headers} ${ftglyph_sources} ${ftfont_sources} ${ftlayout_sources})
target_link_libraries(ftgl ${FREETYPE_LIBRARIES} ${OPENGL_LIBRARY})
set_target_properties(ftgl PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR} LIBRARY_OUTPUT_NAME ftgl)