// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_DIRECTORY_STATICDIRECTORY_H
#define THERON_DETAIL_DIRECTORY_STATICDIRECTORY_H


#include <new>

#include "../../AllocatorManager.h"
#include "../../Assert.h"
#include "../../BasicTypes.h"
#include "../../Defines.h"
#include "../../IAllocator.h"

#include "../../Detail/Directory/Directory.h"
#include "../../Detail/Directory/Entry.h"
#include "../../Detail/Threading/SpinLock.h"


namespace Theron
{
namespace Detail
{


/**
Static class template that manages a reference-counted directory singleton.
*/
template <class Entity>
class StaticDirectory
{
public:

    /**
    Registers an entity and returns its allocated index.
    */
    static uint32_t Register(Entry::Entity *const entity);

    /**
    Deregisters a previously registered entity.
    */
    static void Deregister(const uint32_t index);

    /**
    Gets a reference to the entry with the given index.
    The entry contains data about the entity (if any) registered at the index.
    */
    inline static Entry &GetEntry(const uint32_t index);

private:

    typedef Directory<Entry> DirectoryType;

    static DirectoryType *smDirectory;          ///< Pointer to the allocated instance.
    static SpinLock smSpinLock;                 ///< SpinLock object protecting access.
    static uint32_t smReferenceCount;           ///< Counts the number of entities registered.
};


template <class Entity>
typename StaticDirectory<Entity>::DirectoryType *StaticDirectory<Entity>::smDirectory = 0;

template <class Entity>
SpinLock StaticDirectory<Entity>::smSpinLock;

template <class Entity>
uint32_t StaticDirectory<Entity>::smReferenceCount = 0;


template <class Entity>
uint32_t StaticDirectory<Entity>::Register(Entry::Entity *const entity)
{
    smSpinLock.Lock();

    // Create the singleton instance if this is the first reference.
    if (smReferenceCount++ == 0)
    {
        IAllocator *const allocator(AllocatorManager::Instance().GetAllocator());
        void *const memory(allocator->AllocateAligned(sizeof(DirectoryType), THERON_CACHELINE_ALIGNMENT));

        if (memory == 0)
        {
            return 0;
        }

        smDirectory = new (memory) DirectoryType();
    }

    THERON_ASSERT(smDirectory);

    const uint32_t index(smDirectory->Allocate());

    // Set up the entry.
    if (index)
    {
        Entry &entry(smDirectory->GetEntry(index));
        entry.Lock();
        entry.SetEntity(entity);
        entry.Unlock();
    }

    smSpinLock.Unlock();

    return index;
}


template <class Entity>
void StaticDirectory<Entity>::Deregister(const uint32_t index)
{
    smSpinLock.Lock();

    THERON_ASSERT(smDirectory);
    THERON_ASSERT(index);

    // Clear the entry.
    // If the entry is pinned then we have to wait for it to be unpinned.
    Entry &entry(smDirectory->GetEntry(index));

    bool deregistered(false);
    while (!deregistered)
    {
        entry.Lock();

        if (!entry.IsPinned())
        {
            entry.Free();
            deregistered = true;
        }

        entry.Unlock();
    }

    // Destroy the singleton instance if this was the last reference.
    if (--smReferenceCount == 0)
    {
        IAllocator *const allocator(AllocatorManager::Instance().GetAllocator());
        smDirectory->~DirectoryType();
        allocator->Free(smDirectory, sizeof(DirectoryType));
    }

    smSpinLock.Unlock();
}


template <class Entity>
THERON_FORCEINLINE Entry &StaticDirectory<Entity>::GetEntry(const uint32_t index)
{
    THERON_ASSERT(smDirectory);
    THERON_ASSERT(index);

    return smDirectory->GetEntry(index);
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_DIRECTORY_STATICDIRECTORY_H

