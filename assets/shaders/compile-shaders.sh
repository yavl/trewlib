slangc RawTriangle.vert.slang -target spirv -o compiled/RawTriangle.vert.spv
slangc SolidColor.frag.slang -target spirv -o compiled/SolidColor.frag.spv
slangc RawTriangle.vert.slang -target metal -o compiled/RawTriangle.vert.msl
slangc SolidColor.frag.slang -target metal -o compiled/SolidColor.frag.msl
