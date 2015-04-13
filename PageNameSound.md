# Mangalore 사운드 재생 #

## 1. 지원하는 사운드 포맷은 **WAV** 와 **OGG**. (확장자 **.wav** 와 **.ogg**) ##

## 2. 동시 사운드 재생 지원. 4개까지는 재생이 되는 것을 직접 확인하였고, 아마 그 이상까지도 지원할 듯 ##

## 3. 프로그램 내에서 별도로 소스를 추가하지 않아도 Mangalore 에서 기본적으로 4번 항목의 **sound.xml** 파일을 읽어서 사운드 목록을 작성해 줌 ##

## 4. **data/tables** 폴더에 있는 **sound.xml** 파일을 엑셀로 열어서 수정 ##

  * **Name**: 게임 내에서 사용할 사운드의 이름 (실제 사운드 파일명과는 별개). 다른 사운드의 **Name** 항목과 중복 불가
> > (예) Sound1

  * **File**: 폴더와 확장자를 포함한 실제 파일. 프로그램 내부에서 기본적으로 붙여 주는 상대 경로는 **sound:** 이며 **data/scripts/startup.tcl** 파일에서 별도로 수정하지 않았을 경우 경로는 **export/audio**
> > (예) music/bgm01.ogg    → 이렇게 지정했을 경우 실제 파일 경로는 기본 경로일 경우 export/audio/music/bgm01.ogg

  * **Volume**: 볼륨 기본값. 최소 **0** 부터 최대 **100** 까지. 프로그램 내에서 이 값을 100 으로 나누어 0.0f 부터 1.0f 까지의 값으로 변환되며, 프로그램 내에서 사운드 재생 메시지를 날리면서 볼륨을 설정하는 것도 가능

  * **Stream**: 사운드 재생 시 스트리밍 방식 사용 여부. **true** 또는 **false**. 대소문자 구별 없음. 정확한 기준은 알 수 없으나 용량이 일정 이상 큰 파일의 경우 **true** 로 설정하지 않으면 소리가 아예 안 나는 듯 (4MB 짜리 ogg 로 확인)

  * **Loop**: 반복 재생 여부. **true** 또는 **false**. 대소문자 구별 없음

  * **Ambient**: 환경음 설정 여부? 정확한 용도는 알 수 없으나 아마도 3D 음원 사용 여부인 듯. **true** 또는 **false**. 대소문자 구별 없음. **true** 로 설정하지 않을 경우 파일을 찾을 수 없다고 나오는 현상 발생. 일단은 **true** 로 설정해서 사용할 것. (2009-03-26)

  * **Tracks**: 트랙 번호? 트랙 번호를 이용하여 배경음를 재생할 수 있는 것이 아닌가 했으나 불확실. 혹은 아직까지는 미구현? 1 이상의 값으로 입력. 다른 사운드와 중복 가능 (0 으로 입력하면 로딩 시 에러)

  * **MinDist**: 사운드 영향 범위 최소 거리. 플레이어(카메라)의 위치와 사운드 재생 위치에 따라 이 거리보다 가까울 경우 볼륨 차이가 없는 듯. 이 값과 아래 항목인 MaxDist 사이에서는 볼륨이 점점 줄어들 것으로 예상

  * **MaxDist**: 사운드 영향 범위 최대 거리. 플레이어(카메라)의 위치와 사운드 재생 위치에 따라 이 거리보다 멀 경우 사운드가 아예 재생이 되지 않는 듯

  * **Pri**: 우선 순위? **code/nebula2/inc/audio3/nsound3.h** 파일에 의하면 음성 처리와 관련되어 있는 듯. 기본적으로는 0 이상의 값으로 입력. 다른 사운드와 중복 가능

  * **Category**: 사운드의 종류. **effect**, **music**, **speech**, **ambient** 중에서 한 가지를 입력해야 함 (대소문자 구별 있음. 전부 다 소문자로만 입력해야 함)


## 5. 직접 Audio Server 에 접근해서 사운드를 재생할 경우 ##

  * #include "audio/server.h"
  * Audio::Server::Instance()->PlaySoundEffect(const nString& fxName, const vector3& pos, const vector3& vel, float volume);
> > (예) Audio::Server::Instance()->PlaySoundEffect("Sound1", vector3(0.0f, 0.0f, 0.0f), vector3(0.0f, 0.0f, 0.0f), 1.0f);


## 6. 사운드 재생 메시지를 날릴 경우 ##

  * Mangalore 에 있는 AudioProperty 는 껍데기만 있고 구현되어 있지 않기 때문에 직접 프로퍼티를 작성하고 HandleMessage() 를 재정의하여 처리
  * #include "msg/playsound.h"
  * Mangalore 의 **audio/soundfxhandler.h**, **audio/soundfxhandler.cc** 참고


## 7. Material 간 충돌 사운드를 지정하고 싶을 경우 ##
  * **data/tables/materials.xml** 파일을 엑셀로 열어서 내용을 수정
  * 엑셀 실행 시 아래쪽에 있는 **materials**, **friction**, **bouncyness**, **sound** 시트 중에서 **sound** 시트를 클릭하여 화면에 표시
  * 회색으로 칠해진 칸은 제외하고 나머지 칸에 위의 4번 항목에서 설정한 사운드 이름(**Name**)을 입력하면 게임 내에서 Material 간에 충돌이 일어났을 경우 해당 사운드가 재생
  * 등록되어 있지 않은 사운드 이름일 경우 재생되지 않음 (오류가 나지는 않음)
  * 기본적으로 플레이어의 경우 Material 은 'Character' 로 설정되어 있음