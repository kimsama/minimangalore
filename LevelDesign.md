How to desgine level with 3dmax and mangalore ?

# 1. 지형의 표현 #
3dmax에서 지형 메쉬를 만들고 텍스쳐 매핑을 하면 되는것인가.

# 2. 충돌볼륨 ( 지형,건물,기타 ) #

Physic Volume의 Data를 추출하는 physxexport.ms에서 physxxmlutil.ms 를 include하여
XML데이터를 만들어 내는데 Composites의 's'가 붙는것 numShape에 's'가 안붙는것  </>가 붙는것
등의 에러는 physxxmlutil.ms를 수정한다.


# 3. 레벨에 등장하는 오브젝트의 배치 #
3dmax에서 오브젝트 들을 배치하고 레벨에 등장하는 오브젝트의 DB를 작성해야한다.


**참고: merge와 xref의 차이점**

merge와 xref는 비교할 수 없는 기능적 차이가 있습니다.
merge : A Scene안에 B Scene 을 합치는 기능, 또는 그 반대로 합치는 기능입니다.  B Scene 안에 있는 특정 오브젝트만 합칠수 가 있습니다.
그렇게 하여, 하나의 max파일로 만들어서 작업을 하는것이 목적입니다.
Xref : Xref는 레프렌스파일 (참조파일) 입니다.  Xref 는 Xref Objects와 Xref Scene이 있습니다.
Xref Object는 a 파일에 있는 오브젝트를 b파일에 참조하여, 작업을 하는것입니다.

예를들자면,b.max파일안에 box01 오브젝트를 a.max파일로 xref objects 로 가져가게되면, a.max파일안에서는 box01을 수정할수가 없습니다. box01을 오브젝의 변형을 수정하려면, b.max파일을 열어서, 수정을 해줘야하며, b.max파일에 box01을 수정하면, 자동으로 a.max파일안에 xref로 불러온 box01은 자동으로 업데이트 됩니다.(옵션에서 체크시) xref Scene은 object와 같지만, 원하는 오브젝트가 아니라 원하는 max파일, 즉 Scene을 직접 참조파일로 불러가는 것입니다. 흔히, 배경을 불러오거나, 전체 씬을 불러오고, 디테일한 작업을 할 경우, 사용합니다.

예를들자면, 산이 있고, 그 앞주변에 마을이 있습니다. 마을씬에 그 산을 xref scene로 불러오면, 마을을 작업하는사람은, 마을작업만 하고, 산에대한 작업은, 배경작업자가 하면, 되는 것입니다. 그럼, 배경작업을 하는 사람에게는 자동으로, 산의 수정이 실시간으로 보여지게 되는것입니다.

xref는 팀작업시에 효과적인 방법입니다. 또한 xref는, 프락시로 제어가 가능하기때문에, vray proxy보다 더욱 가볍게 작업을 할수 있다고 봅니다. 위에서 예를 든것처럼, 산의 폴리곤이 많더라도, 자기가 원하는 모델링을 가볍게 만들어서, 그것을 proxy로 사용하는것이 일반적인 방법입니다.

결론은
1. merge는 씬+씬(object)    2. xref는 씬+씬(object) 참조파일.(팀작업시)