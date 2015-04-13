# 3D Max에서 애니메이션되고있는 캐릭터를 게임에 적용하기

# Introduction #
Max파일에 저장되어 애니메이션되고 있는 캐릭터를 게임에 어떻게 적용할것인가.


# Details #
## 준비사항 ##
- 캐릭터 max파일

> Biped 와 Skining 으로 애니메이션되고있는 파일 (COM은 부모가 없어야함)


- anims.xml파일

> 코드(프로그래머)에서의 State 이름과 맥스(아티스트)에서의 TrackNote에 적힌 동작이름을 연결하는 파일 ( data\tables\anims.xml) 에 위치해야하며 mangalpath함수로 참조위치를 확인

> xml파일의 필드를 추가한다. Attr::AnimSet 이름으로 사용할 이름을 추가한다  밑의 예 있음

## 동작 구분하기 ##
> - SchemeticView에서 Com을 선택후 CurveEditor를 열고 DopheSheet로 변환후 TrackNote를 추가한후 Key를 추가선택후 그래프 상에서 왼쪽클릭하여 추가후 삽입된 메모지그림의 오른쪽 클릭후 Note정보를 적는다.

상태이름: clamp         (<- ": clamp"는 1회 재생을 의미하며, 생략하면 반복됨)


duration : 30          (동작의 프레임수)


fadetime : 0.0          (모르겠음)


Max파일에 Note에 기록한 내용은 저장된다.



## 익스포팅 ##
> - 메뉴 Nebula2 -> export , 테스트는 뷰어의 메뉴에서 cha 선택후 동작을 선택한다.

## 게임에 적용하기 ##

프로퍼티의 사용
```
bool DemoGameState::CreatePlayer()
{
	Managers::FactoryManager *factoryManager = Managers::FactoryManager::Instance();
    this->player = factoryManager->CreateEntityByClassName("Entity");


	Ptr<Game::Property> pp = factoryManager->CreateProperty("UePlayerPhysicsProperty");
	this->player->AttachProperty(pp);

	Ptr<Game::Property> ap = factoryManager->CreateProperty("UePlayerAnimationProperty");
	this->player->AttachProperty(ap);

	Ptr<Game::Property> gp = factoryManager->CreateProperty("ActorGraphicsProperty");
	this->player->AttachProperty(gp);
	this->player->SetString(Attr::Graphics, "urbanextreame/ueplayer");
	this->player->SetString(Attr::AnimSet, "UePlayer");	// FieldName in anim.xml 

    Ptr<Game::Property> ip = factoryManager->CreateProperty("UePlayerInputProperty");
    this->player->AttachProperty(ip);

	Ptr<Game::Property> cp = factoryManager->CreateProperty("UePlayerCameraProperty");
	this->player->AttachProperty(cp);

 	Ptr<Game::Property> tp = factoryManager->CreateProperty("TransformableProperty");


    // begin hack
    matrix44 tm;
  
  	this->player->SetMatrix44(Attr::Transform, tm);
  	this->player->AttachProperty(tp);
 
 	Managers::FocusManager::Instance()->SetFocusEntity(this->player);
    Managers::EntityManager::Instance()->AttachEntity(this->player);

    return true;
}
```


애니메이션 바꾸기
```
 
void 
UePlayerAnimationProperty::OnActivate()
{
	Game::Property::OnActivate();

	// by default go to idle animation
	this->RequestAnimation("Idle","", n_rand(0.0f, 1.0f));
}

void
UePlayerAnimationProperty::HandleMessage(Message::Msg* msg)
{
	n_assert(msg);
	if (msg->CheckId(Message::MoveStop::Id))
	{
		this->RequestAnimation("Idle", "", n_rand(0.0f, 1.0f));
	}
	else if (msg->CheckId(Message::MoveDirection::Id) ||
		msg->CheckId(Message::MoveSetVelocity::Id))
	{
		if (this->GetEntity()->HasAttr(Attr::MaxVelocity) && this->GetEntity()->GetFloat(Attr::MaxVelocity) > 1.2f)
		{
			this->RequestAnimation("Run", "", n_rand(0.0f, 1.0f));
		}
		else this->RequestAnimation("Walk", "", n_rand(0.0f, 1.0f));
	}
}
```

Add your content here.  Format your content with:
  * Text in **bold** or _italic_
  * Headings, paragraphs, and lists
  * Automatic links to other wiki pages