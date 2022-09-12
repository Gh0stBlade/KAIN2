SET PATH=%PATH%;C:\Program Files (x86)\SCE\PSP2 SDKs\1.600\host_tools\build\bin

mkdir GXM

psp2cgc -DVERTEX -DGXM --cache --profile sce_vp_psp2 gte_shader_4.hlsl -o GXM/gte_shader_4_vs.gxp
psp2cgc -DFRAGMENT -DGXM --cache --profile sce_fp_psp2 gte_shader_4.hlsl -o GXM/gte_shader_4_fs.gxp

psp2cgc -DVERTEX -DGXM --cache --profile sce_vp_psp2 gte_shader_8.hlsl -o GXM/gte_shader_8_vs.gxp
psp2cgc -DFRAGMENT -DGXM --cache --profile sce_fp_psp2 gte_shader_8.hlsl -o GXM/gte_shader_8_fs.gxp

psp2cgc -DVERTEX -DGXM --cache --profile sce_vp_psp2 gte_shader_16.hlsl -o GXM/gte_shader_16_vs.gxp
psp2cgc -DFRAGMENT -DGXM --cache --profile sce_fp_psp2 gte_shader_16.hlsl -o GXM/gte_shader_16_fs.gxp

psp2cgc -DVERTEX -DGXM --cache --profile sce_vp_psp2 blit_shader.hlsl -o GXM/blit_shader_vs.gxp
psp2cgc -DFRAGMENT -DGXM --cache --profile sce_fp_psp2 blit_shader.hlsl -o GXM/blit_shader_fs.gxp

psp2bin GXM/gte_shader_4_vs.gxp -b2e PSP2,gte_shader_4_vs,gte_shader_4_vs_size,4 -o GXM/gte_shader_4_vs.gxp.obj
psp2bin GXM/gte_shader_4_fs.gxp -b2e PSP2,gte_shader_4_fs,gte_shader_4_fs_size,4 -o GXM/gte_shader_4_fs.gxp.obj

psp2bin GXM/gte_shader_8_vs.gxp -b2e PSP2,gte_shader_8_vs,gte_shader_8_vs_size,4 -o GXM/gte_shader_8_vs.gxp.obj
psp2bin GXM/gte_shader_8_fs.gxp -b2e PSP2,gte_shader_8_fs,gte_shader_8_fs_size,4 -o GXM/gte_shader_8.fs.gxp.obj

psp2bin GXM/gte_shader_16_vs.gxp -b2e PSP2,gte_shader_16_vs,gte_shader_16_vs_size,4 -o GXM/gte_shader_16_vs.gxp.obj
psp2bin GXM/gte_shader_16_fs.gxp -b2e PSP2,gte_shader_16_fs,gte_shader_16_fs_size,4 -o GXM/gte_shader_16.fs.gxp.obj

psp2bin GXM/blit_shader_vs.gxp -b2e PSP2,blit_shader_vs,blit_shader_vs_size,4 -o GXM/blit_shader_vs.gxp.obj
psp2bin GXM/blit_shader_fs.gxp -b2e PSP2,blit_shader_fs,blit_shader_fs_size,4 -o GXM/blit_shader_fs.gxp.obj