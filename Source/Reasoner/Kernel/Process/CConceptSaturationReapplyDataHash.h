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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTSATURATIONREAPPLYDATAHASH_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTSATURATIONREAPPLYDATAHASH_H

// Libraries includes


// Namespace includes
#include "ProcessSettings.h"
#include "CConceptSaturationDescriptorReapplyData.h"


// Other includes
#include "Context/CContext.h"

#include "Utilities/Memory/CMemoryAllocationManager.h"


// Logger includes
#include "Logger/CLogger.h"

namespace Konclude {

	using namespace Context;
	using namespace Utilities::Memory;

	namespace Reasoner {

		namespace Kernel {

			namespace Process {


				/*!
				 *
				 *		\class		CConceptSaturationReapplyDataHash
				 *		\author		Konclude Developer Team
				 *		\version	0.6
				 *		\brief		The hash of a saturation label set from concept tags to their descriptor and reapply data.
				 *
				 *				An open-addressing table with the entries stored inline, in the place of the chained
				 *				hash that allocated a node per entry and rehashed by relinking: the insertions into
				 *				the labels were a fifth of the single saturation thread's time (issue #92). Linear
				 *				probing over a power-of-two array, grown at two thirds load; the array comes from the
				 *				memory allocation manager of the context (the task's pool), like the nodes did.
				 *
				 *				Only what the label set and its iterator use: operator[], tryGetValuePointer, count,
				 *				isEmpty, copy assignment (a deep copy; the shared hash of a copied label is shared by
				 *				pointer, not by this class), detach (nothing to do, kept for the call sites) and a
				 *				const_iterator over the occupied entries in array order.
				 *
				 */
				class CConceptSaturationReapplyDataHash {
					// public methods
					public:
						struct Entry {
							cint64 mKey;
							CConceptSaturationDescriptorReapplyData mValue;
						};

						class const_iterator {
							public:
								inline const_iterator() : mCur(nullptr), mEnd(nullptr) {}
								inline const_iterator(const Entry* cur, const Entry* end) : mCur(cur), mEnd(end) { skipEmpty(); }
								inline cint64 key() const { return mCur->mKey; }
								inline const CConceptSaturationDescriptorReapplyData& value() const { return mCur->mValue; }
								inline const CConceptSaturationDescriptorReapplyData& operator*() const { return mCur->mValue; }
								inline const CConceptSaturationDescriptorReapplyData* operator->() const { return &mCur->mValue; }
								inline const_iterator& operator++() { ++mCur; skipEmpty(); return *this; }
								inline const_iterator operator++(int) { const_iterator r = *this; ++mCur; skipEmpty(); return r; }
								inline bool operator==(const const_iterator& o) const { return mCur == o.mCur; }
								inline bool operator!=(const const_iterator& o) const { return mCur != o.mCur; }
							private:
								inline void skipEmpty() { while (mCur != mEnd && mCur->mKey == EMPTY_KEY) { ++mCur; } }
								const Entry* mCur;
								const Entry* mEnd;
						};

						//! Constructor
						inline CConceptSaturationReapplyDataHash(CContext* context = nullptr) : mEntries(nullptr), mCapacity(0), mShift(64), mCount(0) {
							mMemMan = CContext::getMemoryAllocationManager(context);
						}

						inline CConceptSaturationReapplyDataHash(const CConceptSaturationReapplyDataHash& other) : mEntries(nullptr), mCapacity(0), mShift(64), mCount(0), mMemMan(other.mMemMan) {
							copyFrom(other);
						}

						//! Destructor
						inline ~CConceptSaturationReapplyDataHash() {
							releaseEntries(mEntries);
						}

						//! A deep copy; the allocation manager of this hash is kept
						inline CConceptSaturationReapplyDataHash& operator=(const CConceptSaturationReapplyDataHash& other) {
							if (this != &other) {
								copyFrom(other);
							}
							return *this;
						}

						//! The chained hash shared its data on copy and had to be detached before a write; the copy here is already a copy
						inline void detach() {}

						inline int count() const { return (int)mCount; }
						inline int size() const { return (int)mCount; }
						inline bool isEmpty() const { return mCount == 0; }

						//! The value of the key, inserted default constructed if the key is not present
						inline CConceptSaturationDescriptorReapplyData& operator[](cint64 key) {
							if ((mCount + 1) * 3 > mCapacity * 2) {
								grow();
							}
							Entry* entry = findEntry(key);
							if (entry->mKey == EMPTY_KEY) {
								entry->mKey = key;
								entry->mValue = CConceptSaturationDescriptorReapplyData();
								++mCount;
							}
							return entry->mValue;
						}

						inline bool tryGetValuePointer(cint64 key, CConceptSaturationDescriptorReapplyData*& valuePointer) const {
							if (mCount == 0) {
								return false;
							}
							Entry* entry = findEntry(key);
							if (entry->mKey == EMPTY_KEY) {
								return false;
							}
							valuePointer = &entry->mValue;
							return true;
						}

						inline bool contains(cint64 key) const {
							if (mCount == 0) {
								return false;
							}
							return findEntry(key)->mKey != EMPTY_KEY;
						}

						inline const_iterator constBegin() const { return const_iterator(mEntries, mEntries + mCapacity); }
						inline const_iterator constEnd() const { return const_iterator(mEntries + mCapacity, mEntries + mCapacity); }
						inline const_iterator begin() const { return constBegin(); }
						inline const_iterator end() const { return constEnd(); }

					// private methods
					private:
						static const cint64 EMPTY_KEY = -1;

						inline cint64 indexOf(cint64 key) const {
							return (cint64)(((cuint64)key * 0x9E3779B97F4A7C15ULL) >> mShift);
						}

						//! The entry holding the key, or the empty entry where it would go; requires a capacity with a free slot
						inline Entry* findEntry(cint64 key) const {
							cint64 mask = mCapacity - 1;
							cint64 i = indexOf(key);
							while (true) {
								Entry* entry = mEntries + i;
								if (entry->mKey == key || entry->mKey == EMPTY_KEY) {
									return entry;
								}
								i = (i + 1) & mask;
							}
						}

						inline Entry* allocateEntries(cint64 capacity) {
							Entry* entries = nullptr;
							if (mMemMan) {
								entries = (Entry*)mMemMan->allocate(sizeof(Entry) * capacity);
							} else {
								entries = (Entry*)::operator new(sizeof(Entry) * capacity);
							}
							for (cint64 i = 0; i < capacity; ++i) {
								new (&entries[i]) Entry();
								entries[i].mKey = EMPTY_KEY;
							}
							return entries;
						}

						inline void releaseEntries(Entry* entries) {
							if (entries) {
								if (mMemMan) {
									mMemMan->release(entries);
								} else {
									::operator delete(entries);
								}
							}
						}

						inline void rebuild(cint64 newCapacity) {
							Entry* oldEntries = mEntries;
							cint64 oldCapacity = mCapacity;
							cint64 shift = 64;
							for (cint64 c = newCapacity; c > 1; c >>= 1) {
								--shift;
							}
							mEntries = allocateEntries(newCapacity);
							mCapacity = newCapacity;
							mShift = shift;
							for (cint64 i = 0; i < oldCapacity; ++i) {
								if (oldEntries[i].mKey != EMPTY_KEY) {
									Entry* entry = findEntry(oldEntries[i].mKey);
									entry->mKey = oldEntries[i].mKey;
									entry->mValue = oldEntries[i].mValue;
								}
							}
							releaseEntries(oldEntries);
						}

						inline void grow() {
							rebuild(mCapacity == 0 ? 8 : mCapacity * 2);
						}

						inline void copyFrom(const CConceptSaturationReapplyDataHash& other) {
							Entry* oldEntries = mEntries;
							mEntries = nullptr;
							mCapacity = 0;
							mShift = 64;
							mCount = 0;
							if (other.mCapacity > 0) {
								mEntries = allocateEntries(other.mCapacity);
								mCapacity = other.mCapacity;
								mShift = other.mShift;
								mCount = other.mCount;
								for (cint64 i = 0; i < mCapacity; ++i) {
									mEntries[i] = other.mEntries[i];
								}
							}
							releaseEntries(oldEntries);
						}

					// private variables
					private:
						Entry* mEntries;
						cint64 mCapacity;
						cint64 mShift;
						cint64 mCount;
						CMemoryAllocationManager* mMemMan;
				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CCONCEPTSATURATIONREAPPLYDATAHASH_H
