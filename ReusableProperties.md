# 개요 #

각 프로젝트들간에 재사용가능한 프로퍼티(Property)들에 대한 내용. 프로그램을 크게 "Menu State"와 "Game State"의 두 상태로 정의하고 각 상태에서 프로젝트간에 재사용 가능한 프로퍼티들을 정의한다.

재사용 가능한 프로퍼티들을 더 많이 확보할 수록 프로젝트의 생존율이 높아진다!

# Menu State #

Menu State는 프로퍼티뿐만 아니라 메뉴 상태와 메뉴 아이템 모두 프로젝트간에 공유가 가능하다.

Menu Item Entity
  * Menu 상태의 메뉴 아이템들.

# Game State #

  * '''Timer''' Property
  * 시간 타이머 - 트랩, 롤러블레이드, 퍼즐 도미노, 장님 안내 등의 프로젝트에서 재사용 가능
  * '''Abstract\_Trigger''' Property
  * 
  * 자동차와 같은 오브젝트의 경우 PhysicsProperty를 상속한 CarPhysicsProperty의 정의가 필요하다.