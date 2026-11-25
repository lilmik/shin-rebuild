"""Local-only automatic tracing preview; does not replace game assets."""
import sys,struct,json,xml.etree.ElementTree as ET
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'build/vector_tools'))
from PIL import Image
import vtracer
root=Path(__file__).resolve().parents[1];out=root/'artwork/menu_vector_preview_detail';out.mkdir(parents=True,exist_ok=True)
b=(root/'build/flac_assets/RESOURCE.RES').read_bytes();entries={}
for i in range(struct.unpack_from('<I',b)[0]):
 name,off,size=struct.unpack_from('<20sII',b,4+i*28);entries[name.split(b'\0')[0].decode('cp932').lower()]=b[off:off+size]
pal=entries['tt0002.pal'];pal=[tuple(min(255,c*4) for c in pal[i:i+3]) for i in range(0,768,3)]
placements={'tt0002.app':(50,20),'tt0000.app':(50,160),'menu_g1.app':(460,180),'option2.app':(458,216),'menu_e0.app':(460,260)}
names=list(placements)+['menu_g0.app','option1.app','menu_e1.app'];images={};rows=[]
for name in names:
 data=entries[name];w,h,n=struct.unpack_from('<III',data);pixels=bytearray()
 for i in range(12,len(data),2):pixels.extend([data[i]]*data[i+1])
 assert len(pixels)==w*h
 im=Image.new('RGBA',(w,h));im.putdata([pal[c]+(0 if c==254 else 255,) for c in pixels]);images[name]=im;png=out/(name+'.png');im.save(png)
 trace=out/(name+'.trace.png');im.resize((w*4,h*4),Image.Resampling.NEAREST).save(trace)
 svg=out/(name+'.svg');vtracer.convert_image_to_svg_py(str(trace),str(svg),colormode='color',hierarchical='stacked',mode='spline',filter_speckle=1,color_precision=8,layer_difference=8,corner_threshold=75,length_threshold=4.0,max_iterations=10,splice_threshold=45,path_precision=3)
 rows.append({'asset':name,'size':[w,h],'svg_bytes':svg.stat().st_size})
canvas=Image.new('RGBA',(640,480),'white');ns='http://www.w3.org/2000/svg';ET.register_namespace('',ns);doc=ET.Element('{'+ns+'}svg',{'width':'1920','height':'1440','viewBox':'0 0 640 480'});ET.SubElement(doc,'{'+ns+'}rect',{'width':'640','height':'480','fill':'white'})
# Composite only the extracted foreground; wallpaper intentionally excluded for comparison.
for name,(x,y) in placements.items():
 canvas.alpha_composite(images[name],(x,y));svg=ET.parse(out/(name+'.svg')).getroot();svg.set('viewBox',f'0 0 {images[name].width*4} {images[name].height*4}');svg.set('width',str(images[name].width));svg.set('height',str(images[name].height));svg.set('x',str(x));svg.set('y',str(y));doc.append(svg)
canvas.resize((1920,1440),Image.Resampling.NEAREST).convert('RGB').save(out/'original_3x.png');ET.ElementTree(doc).write(out/'menu_vector.svg',encoding='utf-8',xml_declaration=True)
(out/'manifest.json').write_text(json.dumps(rows,ensure_ascii=False,indent=2),encoding='utf-8');print(out)
