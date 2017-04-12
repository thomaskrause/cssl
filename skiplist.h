/*
 * Copyright 2017 Stefan Sprenger <sprengsz@informatik.hu-berlin.de>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __SkipList_H
#define __SkipList_H

#define MAX_SKIP 5
// initial size of the highest fast lane with number
// of keys that fit into one cache line
#define TOP_LANE_BLOCK 16
// number of keys that can be stored in one SIMD register
#define SIMD_SEGMENTS 8

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <immintrin.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

// data list node
typedef struct DataNode {
  uint32_t key;
  struct DataNode *next;
} DataNode;

// proxy node
typedef struct {
  uint32_t keys[MAX_SKIP];
  DataNode* pointers[MAX_SKIP];
  bool updated;
} ProxyNode;

typedef struct {
  uint8_t     max_level;
  uint8_t     skip;
  uint32_t    num_elements;
  uint32_t*   items_per_level;
  uint32_t*   flane_items;
  uint32_t*   starts_of_flanes;
  uint32_t*   flanes;
  ProxyNode** flane_pointers;
  DataNode    *head, *tail;
} SkipList;

// result of a range query
typedef struct {
  size_t startIdx;
  size_t endIdx;
  uint32_t  count;
} RangeSearchResult;

SkipList*         createSkipList(uint8_t maxLevel, uint8_t skip);
void              insertElement(SkipList* slist, uint32_t key);
uint32_t          insertItemIntoFastLane(SkipList* slist, int8_t level, DataNode* newNode);
void              buildFastLanes(SkipList* slist);
void              resizeFastLanes(SkipList* slist);
uint32_t          searchElement(SkipList* slist, uint32_t key);
RangeSearchResult searchRange(SkipList* slist, uint32_t startKey, uint32_t endKey);
DataNode*         newNode(uint32_t key);
#endif
