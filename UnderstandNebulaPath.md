# Introduction #

Add your content here.


# Details #

각종 경로 정리

1. home:

네뷸라에서는 처음 home을 assign하기위해 실행파일의 절대경로를 얻어 home으로 사용한다

(GetCurrentDirectory가 아닌 GetModuleFileName을 사용한다)

예외적으로 파일이름 앞의 경로 이름이 win32,win32d 가 존재하면 상위 두단계를 삭제하여 home으로 설정한다.

예를들면  D:\dev\nebula2\bin\win32\ -> D:\dev\nebula2    ,  D:\dev\nebula2\bin\win32d  ->
D:\dev\nebula2 변한다.

참고로 빌드 파일설정에서 실행파일 생성위치를 nebula\bin가 아니라 따로 정할수 있다.
```
#-----------------------------------------------------------------------------
beginworkspace urbanextremeworkspace
	setbinarydir {./code/urbanextreme/bin/ }
    settargets {
        urbanextreme
        mangalore
    }
endworkspace
#-----------------------------------------------------------------------------
```

2. proj:

app에서 proj경로를 따로 설정하지않았으면 home으로 assign되며

app시작시 proj경로를 확인후 설정되어있으면 proj:data/scripts/startup.tcl를 읽고 아니면

home:data/scripts/startup.tcl 읽는다.

참고로 스타트업패스를 설정해도 설정한곳에서 읽지않는다.

startup.tcl 에서는 아래의 경로를 설정할수 있다.
```
    .setassign "data"     "$proj/data/"
    .setassign "export"   "$proj/export/"

    .setassign "renderpath" "data:shaders"
    .setassign "scripts"    "data:scripts"
    .setassign "physics"    "export:physics/"
    .setassign "meshes"     "export:meshes/"
    .setassign "textures"   "export:textures/"
    .setassign "anims"      "export:anims/"
    .setassign "gfxlib"     "export:gfxlib/"
    .setassign "lights"     "export:lightlib/"
    .setassign "db"         "export:db/"
    .setassign "levels"     "export:levels/"
    .setassign "sound"      "export:audio/"
    .setassign "cegui"      "export:cegui/"
    .setassign "charlib"    "gfxlib:characters/"
```

3. nviewer경로

nviewer는 커맨드 옵션 -projdir 을 따로 설정하지많으면 nviewer기준의 home을 경로로 사용한다.