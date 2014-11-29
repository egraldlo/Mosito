// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_MESSAGES_MESSAGESENDER_H
#define THERON_DETAIL_MESSAGES_MESSAGESENDER_H


#include "../../Address.h"
#include "../../Assert.h"
#include "../../BasicTypes.h"
#include "../../Defines.h"
#include "../../IAllocator.h"
#include "../../Receiver.h"

#include "../../Detail/Directory/Entry.h"
#include "../../Detail/Directory/StaticDirectory.h"
#include "../../Detail/Handlers/FallbackHandlerCollection.h"
#include "../../Detail/Messages/IMessage.h"
#include "../../Detail/Mailboxes/Mailbox.h"
#include "../../Detail/MailboxProcessor/Processor.h"


namespace Theron
{


class EndPoint;


namespace Detail
{


/**
Helper class that sends allocated internal message objects.
*/
class MessageSender
{
public:

    /**
    Sends an allocated message to the given address.
    */
    static bool Send(
        EndPoint *const endPoint,
        Processor::Context *const processorContext,
        const uint32_t localFrameworkIndex,
        IMessage *const message,
        const Address &address,
        const bool localQueue = false);

    /**
    Delivers an allocated message to a receiver or an actor in some framework within this process.
    This function is non-inlined so serves as a call-point to avoid excessive inlining.
    */
    static bool DeliverWithinLocalProcess(
        IMessage *const message,
        const Index &index);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_MESSAGES_MESSAGESENDER_H
