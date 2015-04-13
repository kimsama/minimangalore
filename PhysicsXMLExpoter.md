# Physics XML Expoter #
Physic Shape를 XML 로 추출하기

# 설치 방법 #
1. 우선 SVN Update 로 .ms 수정 파일을 받습니다.

2. 자신의 Nebula2 설치 폴더로 들어갑니다.
  * \trunk\nebula2\code\contrib\nmax\maxscripts\mangalore\scripts\mangalore
  * build.bat 파일을 실행합니다.

3. 아래 경로에 폴더들 복사
  * \trunk\nebula2\code\contrib\nmax\maxscripts\mangalore
  * plugcfg , scripts 폴더를  3DMAX가 설치된 폴더에 복사
4. C:\Program Files\Autodesk\3ds Max 9\Scripts\mangalore\physxhelper.ms 파일을 열고
  * xmlpath = "D:\materials.xml" 를 자신의 nebula2\data\tables\materials.xml 로 수정합니다.
  * 자신의 프로젝트에 materials.xml을 사용시 자신의 프로젝트 폴더 안에 \data\tables\materials.xml로 합니다.

  * 예)
```
    xmlpath = getINISetting "../plugcfg/mangalore.ini" "Material" "xmlpath"
    xmlpath = "D:/dev/nebula2/code/vanquisher/data/tables/materials.xml"
    excel.workbooks.open xmlpath
```

# 사용 방법 #

1. 3D MAX에서 Physic Shape를 사용할 오브젝트를 클릭후 오른쪽 탭 메뉴에 "modify"를 클릭하고
"Modifier List" 리스트박스에서 "ODE Body (Mangalore)" 를 선택합니다.
선택후 나오는 메뉴에서 "Rigid Body"란에서 Static 체크후 Body Shape에서 알맞게 선택합니다.

2. 이제 Physic Shape를 생성하기 위해 오른쪽 탭 메뉴에서 첫번째 커서모양인 "Create"를 클릭하고
"Standard Primitives" 리스트박스에서 "ODE Shape (Mangalore)"를 선택 합니다.
"Object Type" 메뉴에서 Shape 종류를 선택하고 나오는 메뉴에는 각자  사용할 Physic Shape옵션을
선택하고 오브젝트를 적당히 감싸는 Shape를 마우스 또는 "Keyboard Entity"로 만들어 줍니다.

3. 3D MAX 메뉴에서 "Graph Editors"->"New Schematic View"를 클릭하고
툴바에서 3번째 메뉴인 "Connect"를 선택해서  Physic Shape가 감싸는 오브젝트의 자식으로 연결

4. 3D MAX 오른쪽 탭 메뉴에서 "Utilities"를 클릭 "Mangalore Physics Expoter"에 "+"를 클릭후
메뉴를 늘린후 Expoter를 클릭하면 XML을 출력합니다.

- 추가 (2009-03-12) -

1. 위의 2번에서 "ODE Shape (Mangalore)"로 Box를 그릴 때, 오브젝트의 축이 오브젝트의 가운데에 오도록 해야 함. (Mangalore에서 충돌 영역을 만들 때 축을 중심으로 만들어지는 듯)
충돌 영역으로 사용할 도형을 선택 후 오른쪽 탭 메뉴의 "Hierarchy"에서 "Move/Rotate/Scale" 항목의 "Affect Pivot Only"를 클릭한 다음 "Alignment" 의 "Center to Object"를 클릭하면 됨. (수동으로 이동하는 것도 가능)

2. "ODE Shape (Mangalore)"에서 Box 이외의 다른 도형, 즉 "Sphere"나 "Capsule"을 만들어서 Export하게 되면 성공적으로 추출이 되지만, 프로젝트에서 추출한 Physics용 xml 파일을 불러오게 되면 에러 발생. 해결 방법은 위에서 설치한 MaxScript 중 "physxexport.ms" 파일의 내용에서 다음 부분을 수정.

  * ExportODESphere 함수에서 xmlElementBegin "SphereBody" 를 xmlElementBegin "SphereShape" 로 수정
  * ExportODECapsule 함수에서 xmlElementBegin "CapsuleBody" 를 xmlElementBegin "CapsuleShape" 로 수정
  * 이미 추출한 xml 파일의 경우 "SphereBody" 와 "CapsuleBody" 에서 "Body"만 "Shape"로 수정해도 됨)

  * 저장소에 수정된 "physxexport.ms"파일 commit 하였음

3. XML로 추출한 Physic Shape를 사용하면 오브젝트보다 심하게 크게 나오는 현상은 3DMAX 오른쪽 탭메뉴에서 "Utilities"란 에서 "More..." 버튼을 클릭 리스트에서 "Nebula2"를 선택하고 나오는 메뉴에서 "Scale Geometry By"에 0.01로 되있는 수치를 1로 수정하던가 "physxexport.ms"에 파일 내용에 사이즈 추출부분 값을 1/100을 한후 추출하던가 해야 합니다.

  * GetGeomScaleValue함수를 노출시켜서 physexport.ms에서 스케일 적용된 값으로 추출되게 수정하였음


---

<---------- 틀리거나 빠진부분은 계속 수정 --------------->






