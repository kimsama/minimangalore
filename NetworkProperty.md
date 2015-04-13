#Network Property

# 네트워크 모듈 만들기 #

RakNet을 사용한 네트워크 모듈 만들기.

RakNet(http://www.jenkinssoftware.com/)은 p2p용 네트워크 라이브러리.

Nebula3에서는 RakNet을 이용해서 Network을 구현하는데 N2에서도 유사한 방법으로 네트워크 멀티 플레이를 구현할 수 있다. (N2와 N3의 게임 프레임워크 - Mangalore는 매우 유사)

N3는 google code나 The Brain Dump 블로그 사이트(http://flohofwoe.blogspot.com/2008/09/nebula3-september-sdk.html)에서 구할 수 있다.

# 구현 #

모듈위치
  * code/addons/network/
  * code/application/networkfeature/

DefaultClientHandler::ProcessIncomingStreams 함수와 DefaultClientHandler::HandleSystemMessage 함수가 핵심! Managlore의 메시지 시스템을 이해하면 아래 네트워크 메시지 처리도 어렵지 않게 처리할 수 있다. 중요한 것은 기존의 메시지에 메시지 아이디를 추가해서 네트워크 메시지로 이용한다는 것. switch 문이나 if-else 문으로 패킷을 처리하지 않는다.

```
void 
DefaultClientHandler::ProcessIncomingStreams()
{
    n_assert(this->client.isvalid());
    n_assert(this->client->GetNumberRecvStreams() > 0);
    IndexT streamIdx;
    for (streamIdx = 0; streamIdx < this->client->GetNumberRecvStreams(); streamIdx++)
    {
        const Ptr<NetStream>& stream = this->client->GetStreamAtIndex(streamIdx);
        Ptr<Messaging::Message> msg = NetStreamFactory::CreateMessageFromStream(stream);
        this->incomingMsgs.Append(msg);
    }

    /// send new created msgs to ports which want this typ of message
    const Array<Ptr<Messaging::Message> >& msgs = this->incomingMsgs;
    IndexT msgIdx;
    for (msgIdx = 0; msgIdx < msgs.Size(); msgIdx++)
    {
        const Messaging::Id& mId = msgs[msgIdx]->GetId();
        if (this->portMessageIdMapping.Contains(&mId))
        {
            // send message to all registered port for this type of msg
            IndexT portIdx;
            for (portIdx = 0; portIdx < this->portMessageIdMapping[&mId].Size(); portIdx++)
            {
                this->portMessageIdMapping[&mId][portIdx]->Send(msgs[msgIdx]);
            }
        }
    }
}
```

```
bool 
DefaultClientHandler::HandleSystemMessage(const Ptr<Messaging::Message>& msg)
{
    // check for system msgs
    if (msg->CheckId(Connect::Id))
    {
        Ptr<Connect> connectMsg = msg.cast<Connect>();
        this->ConnectToServer(connectMsg->GetServerAddress());
        msg->SetHandled(true);

        return true;
    }
    else if (msg->CheckId(Disconnect::Id))
    {
        this->DisconnectFromServer();
        msg->SetHandled(true);

        return true;
    }
    else if (msg->CheckId(IsClientConnected::Id))
    {
        Ptr<IsClientConnected> isConnected = msg.cast<IsClientConnected>();
        isConnected->SetIsConnected(this->client->IsConnected());
        isConnected->SetHandled(true);

        return true;
    }
    return false;
}
```