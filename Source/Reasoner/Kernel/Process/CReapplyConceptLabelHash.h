/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELHASH_H

// Libraries includes
#include <cstring>

// Namespace includes
#include "ProcessSettings.h"
#include "CConceptDescriptorDependencyReapplyData.h"
#include "CProcessContext.h"

// Other includes
#include "Utilities/Memory/CMemoryAllocationManager.h"
#include "Utilities/Memory/CMemoryAllocator.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*!
				 *
				 *		\class		CReapplyConceptLabelHash
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		The hash from concept tags to the concept descriptors and reapplication queues that make
				 *					up a tableau node's label (CReapplyConceptLabelSet).
				 *
				 *					Open addressing with linear probing over one array of entries, power-of-two capacity,
				 *					Fibonacci hashing of the tag, at most half full (the clash checks and insertions are mostly probe misses), as CReapplyConceptSaturationLabelHash
				 *					does for the saturation. Besides the entries the hash keeps the tags in a sorted array,
				 *					maintained on insertion (binary search and a memmove of the tail), because the blocking,
				 *					caching and merging code walks two labels in tag order; the sorted CPROCESSMAP this replaces
				 *					allocated a skip-list node per entry and duplicated them one by one on each copy.
				 *
				 *					Tags are never removed from a label. The memory comes from the process context's allocation
				 *					manager and is never freed individually, so an outgrown array goes to a free list in the
				 *					context (one per power-of-two capacity, separate lists for entries and tags) and serves the
				 *					next label that grows to that capacity. Tags are never negative, so -1 marks a free slot.
				 *
				 */
				class CReapplyConceptLabelHash {
					// public methods
					public:
						struct Entry {
							cint64 key;
							CConceptDescriptorDependencyReapplyData value;
						};

						static const cint64 EMPTYKEY = -1;
						static const cint64 INITIALCAPACITY = 16;

						//! Constructor
						inline CReapplyConceptLabelHash(CProcessContext* processContext = nullptr) : mEntries(nullptr), mCapacity(0), mSize(0), mShift(64), mKeys(nullptr), mKeyCapacity(0), mMemMan(nullptr), mFreeEntryArrays(nullptr), mFreeKeyArrays(nullptr) {
							if (processContext) {
								mMemMan = processContext->getUsedMemoryAllocationManager();
								mFreeEntryArrays = processContext->getTableauLabelHashFreeArrays(CProcessContext::TABLEAULABELHASHENTRYARRAYS);
								mFreeKeyArrays = processContext->getTableauLabelHashFreeArrays(CProcessContext::TABLEAULABELHASHKEYARRAYS);
							}
						}

						inline cint64 count() const {
							return mSize;
						}

						inline bool isEmpty() const {
							return mSize == 0;
						}

						inline cint64 capacity() const {
							return mCapacity;
						}

						//! The tags of the entries in ascending order, count() of them.
						inline const cint64* sortedKeys() const {
							return mKeys;
						}

						//! The data for the tag, created with a null descriptor and an empty queue if the tag is new.
						inline CConceptDescriptorDependencyReapplyData& operator[](cint64 key) {
							Q_ASSERT(key >= 0);
							Entry* entry = nullptr;
							if (mSize > 0) {
								entry = probe(key);
								if (entry->key == key) {
									return entry->value;
								}
							}
							return insertNew(key,entry)->value;
						}

						inline bool tryGetValuePointer(cint64 key, CConceptDescriptorDependencyReapplyData*& valuePointer) const {
							if (mSize == 0) {
								return false;
							}
							Entry* entry = probe(key);
							if (entry->key == EMPTYKEY) {
								return false;
							}
							valuePointer = &entry->value;
							return true;
						}

						inline bool contains(cint64 key) const {
							CConceptDescriptorDependencyReapplyData* valuePointer = nullptr;
							return tryGetValuePointer(key,valuePointer);
						}

						//! Inserts the value if the tag is new; returns the data of the tag (the existing one if it was contained).
						inline CConceptDescriptorDependencyReapplyData* tryInsert(cint64 key, const CConceptDescriptorDependencyReapplyData& value, bool* containsAlready) {
							Q_ASSERT(key >= 0);
							Entry* entry = nullptr;
							if (mSize > 0) {
								entry = probe(key);
								if (entry->key == key) {
									if (containsAlready) {
										*containsAlready = true;
									}
									return &entry->value;
								}
							}
							if (containsAlready) {
								*containsAlready = false;
							}
							entry = insertNew(key,entry);
							entry->value = value;
							return &entry->value;
						}

						//! Replaces the contents with a copy of the other hash's entries and tags, keeping the layout, so no rehashing.
						inline CReapplyConceptLabelHash* copyFrom(const CReapplyConceptLabelHash& other) {
							if (this == &other) {
								return this;
							}
							if (mEntries) {
								releaseEntryArray(mEntries,mCapacity);
								mEntries = nullptr;
							}
							if (mKeys) {
								releaseKeyArray(mKeys,mKeyCapacity);
								mKeys = nullptr;
							}
							mCapacity = other.mCapacity;
							mSize = other.mSize;
							mShift = other.mShift;
							mKeyCapacity = other.mKeyCapacity;
							if (mCapacity > 0) {
								mEntries = allocateEntryArray(mCapacity);
								memcpy((void*)mEntries,(const void*)other.mEntries,sizeof(Entry)*mCapacity);
							}
							if (mKeyCapacity > 0) {
								mKeys = allocateKeyArray(mKeyCapacity);
								memcpy((void*)mKeys,(const void*)other.mKeys,sizeof(cint64)*mSize);
							}
							return this;
						}

						//! Removes all entries, keeping nothing allocated.
						inline void clear() {
							if (mEntries) {
								releaseEntryArray(mEntries,mCapacity);
								mEntries = nullptr;
							}
							if (mKeys) {
								releaseKeyArray(mKeys,mKeyCapacity);
								mKeys = nullptr;
							}
							mCapacity = 0;
							mSize = 0;
							mShift = 64;
							mKeyCapacity = 0;
						}

						//! Makes room for at least count entries without growing again.
						inline void reserve(cint64 count) {
							cint64 capacity = mCapacity > 0 ? mCapacity : INITIALCAPACITY;
							while (count * 2 > capacity) {
								capacity *= 2;
							}
							if (capacity != mCapacity) {
								rehash(capacity);
							}
							cint64 keyCapacity = mKeyCapacity > 0 ? mKeyCapacity : INITIALCAPACITY;
							while (count > keyCapacity) {
								keyCapacity *= 2;
							}
							if (keyCapacity != mKeyCapacity) {
								growKeys(keyCapacity);
							}
						}

						//! Iteration over the entries in hash (not tag) order.
						inline Entry* entriesBegin() const {
							return mEntries;
						}
						inline Entry* entriesEnd() const {
							return mEntries + mCapacity;
						}

					// protected methods
					protected:
						//! The slot of the key, or the free slot where it would be inserted; the array is never full.
						inline Entry* probe(cint64 key) const {
							cint64 index = (cint64)(((quint64)key * Q_UINT64_C(0x9E3779B97F4A7C15)) >> mShift);
							const cint64 mask = mCapacity - 1;
							for (;;) {
								Entry* entry = mEntries + index;
								if (entry->key == key || entry->key == EMPTYKEY) {
									return entry;
								}
								index = (index + 1) & mask;
							}
						}

						//! Inserts the new key into the free slot found by probe (re-probed if the array has to grow) and into the sorted tags.
						inline Entry* insertNew(cint64 key, Entry* freeEntry) {
							if ((mSize + 1) * 2 > mCapacity) {
								grow();
								freeEntry = nullptr;
							}
							if (!freeEntry) {
								freeEntry = probe(key);
							}
							freeEntry->key = key;
							freeEntry->value.mConceptDescriptor = nullptr;
							freeEntry->value.mPosNegReapplyQueue = CCondensedReapplyQueue();
							insertSortedKey(key);
							++mSize;
							return freeEntry;
						}

						inline void insertSortedKey(cint64 key) {
							if (mSize >= mKeyCapacity) {
								growKeys(mKeyCapacity > 0 ? mKeyCapacity * 2 : INITIALCAPACITY);
							}
							// upper bound of the key, usually at the end because concepts tend to arrive in ascending tag order
							cint64 pos = mSize;
							if (pos > 0 && mKeys[pos - 1] > key) {
								cint64 low = 0;
								cint64 high = pos;
								while (low < high) {
									cint64 mid = (low + high) >> 1;
									if (mKeys[mid] < key) {
										low = mid + 1;
									} else {
										high = mid;
									}
								}
								pos = low;
								memmove((void*)(mKeys + pos + 1),(const void*)(mKeys + pos),sizeof(cint64)*(mSize - pos));
							}
							mKeys[pos] = key;
						}

						inline void grow() {
							rehash(mCapacity > 0 ? mCapacity * 2 : INITIALCAPACITY);
						}

						inline void rehash(cint64 newCapacity) {
							Entry* oldEntries = mEntries;
							cint64 oldCapacity = mCapacity;
							mEntries = allocateEntryArray(newCapacity);
							mCapacity = newCapacity;
							mShift = 64 - log2OfCapacity(newCapacity);
							for (cint64 i = 0; i < newCapacity; ++i) {
								mEntries[i].key = EMPTYKEY;
								mEntries[i].value.mConceptDescriptor = nullptr;
								mEntries[i].value.mPosNegReapplyQueue = CCondensedReapplyQueue();
							}
							for (cint64 i = 0; i < oldCapacity; ++i) {
								if (oldEntries[i].key != EMPTYKEY) {
									*probe(oldEntries[i].key) = oldEntries[i];
								}
							}
							if (oldEntries) {
								releaseEntryArray(oldEntries,oldCapacity);
							}
						}

						inline void growKeys(cint64 newKeyCapacity) {
							cint64* oldKeys = mKeys;
							cint64 oldKeyCapacity = mKeyCapacity;
							mKeys = allocateKeyArray(newKeyCapacity);
							mKeyCapacity = newKeyCapacity;
							if (oldKeys) {
								memcpy((void*)mKeys,(const void*)oldKeys,sizeof(cint64)*mSize);
								releaseKeyArray(oldKeys,oldKeyCapacity);
							}
						}

						static inline int log2OfCapacity(cint64 capacity) {
							int bits = 0;
							for (; capacity > 1; capacity >>= 1) {
								++bits;
							}
							return bits;
						}

						//! An array for the capacity, from the context's free list if one was outgrown earlier, else from the pools.
						inline void* allocateArray(void** freeArrays, cint64 capacity, size_t elementSize) {
							if (freeArrays) {
								void** freeList = freeArrays + log2OfCapacity(capacity);
								if (*freeList) {
									void* array = *freeList;
									*freeList = *(void**)array;
									return array;
								}
							}
							return CMemoryAllocator::allocateMemory(mMemMan,elementSize*capacity);
						}

						//! Keeps the outgrown array for the next hash that needs this capacity (the pools cannot take it back).
						inline void releaseArray(void** freeArrays, void* array, cint64 capacity) {
							if (freeArrays) {
								void** freeList = freeArrays + log2OfCapacity(capacity);
								*(void**)array = *freeList;
								*freeList = array;
							} else {
								CMemoryAllocator::releaseMemory(array,mMemMan);
							}
						}

						inline Entry* allocateEntryArray(cint64 capacity) {
							return (Entry*)allocateArray(mFreeEntryArrays,capacity,sizeof(Entry));
						}
						inline void releaseEntryArray(Entry* entries, cint64 capacity) {
							releaseArray(mFreeEntryArrays,(void*)entries,capacity);
						}
						inline cint64* allocateKeyArray(cint64 capacity) {
							return (cint64*)allocateArray(mFreeKeyArrays,capacity,sizeof(cint64));
						}
						inline void releaseKeyArray(cint64* keys, cint64 capacity) {
							releaseArray(mFreeKeyArrays,(void*)keys,capacity);
						}

					// protected variables
					protected:
						Entry* mEntries;
						cint64 mCapacity;
						cint64 mSize;
						int mShift;
						cint64* mKeys;
						cint64 mKeyCapacity;
						CMemoryAllocationManager* mMemMan;
						void** mFreeEntryArrays;
						void** mFreeKeyArrays;

					// private methods
					private:
						CReapplyConceptLabelHash(const CReapplyConceptLabelHash& other);
						CReapplyConceptLabelHash& operator=(const CReapplyConceptLabelHash& other);

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CREAPPLYCONCEPTLABELHASH_H
