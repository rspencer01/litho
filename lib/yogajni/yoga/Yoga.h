/**
 * Copyright (c) 2014-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

// Not defined in MSVC++
#ifndef NAN
static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
#define NAN (*(const float *) __nan)
#endif

#define YGUndefined NAN

#include "YGEnums.h"
#include "YGMacros.h"

YG_EXTERN_C_BEGIN

typedef struct YGSize {
  float width;
  float height;
} YGSize;

typedef struct YGValue {
  float value;
  YGUnit unit;
} YGValue;

static const YGValue YGValueUndefined = {YGUndefined, YGUnitUndefined};
static const YGValue YGValueAuto = {YGUndefined, YGUnitAuto};

typedef struct YGConfig *YGConfigRef;
typedef struct YGNode *YGNodeRef;
typedef YGSize (*YGMeasureFunc)(YGNodeRef node,
                                float width,
                                YGMeasureMode widthMode,
                                float height,
                                YGMeasureMode heightMode);
typedef float (*YGBaselineFunc)(YGNodeRef node, const float width, const float height);
typedef void (*YGPrintFunc)(YGNodeRef node);
typedef int (*YGLogger)(YGLogLevel level, const char *format, va_list args);

typedef void *(*YGMalloc)(size_t size);
typedef void *(*YGCalloc)(size_t count, size_t size);
typedef void *(*YGRealloc)(void *ptr, size_t size);
typedef void (*YGFree)(void *ptr);

// YGNode
WIN_EXPORT YGNodeRef YGNodeNew(void);
WIN_EXPORT YGNodeRef YGNodeNewWithConfig(const YGConfigRef config);
WIN_EXPORT void YGNodeFree(const YGNodeRef node);
WIN_EXPORT void YGNodeFreeRecursive(const YGNodeRef node);
WIN_EXPORT void YGNodeReset(const YGNodeRef node);
WIN_EXPORT int32_t YGNodeGetInstanceCount(void);

WIN_EXPORT void YGNodeInsertChild(const YGNodeRef node,
                                  const YGNodeRef child,
                                  const uint32_t index);
WIN_EXPORT void YGNodeRemoveChild(const YGNodeRef node, const YGNodeRef child);
WIN_EXPORT YGNodeRef YGNodeGetChild(const YGNodeRef node, const uint32_t index);
WIN_EXPORT YGNodeRef YGNodeGetParent(const YGNodeRef node);
WIN_EXPORT uint32_t YGNodeGetChildCount(const YGNodeRef node);

WIN_EXPORT void YGNodeCalculateLayout(const YGNodeRef node,
                                      const float availableWidth,
                                      const float availableHeight,
                                      const YGDirection parentDirection);

// Mark a node as dirty. Only valid for nodes with a custom measure function
// set.
// YG knows when to mark all other nodes as dirty but because nodes with
// measure functions
// depends on information not known to YG they must perform this dirty
// marking manually.
WIN_EXPORT void YGNodeMarkDirty(const YGNodeRef node);
WIN_EXPORT bool YGNodeIsDirty(const YGNodeRef node);

WIN_EXPORT void YGNodePrint(const YGNodeRef node, const YGPrintOptions options);

WIN_EXPORT bool YGFloatIsUndefined(const float value);

WIN_EXPORT bool YGNodeCanUseCachedMeasurement(const YGMeasureMode widthMode,
                                              const float width,
                                              const YGMeasureMode heightMode,
                                              const float height,
                                              const YGMeasureMode lastWidthMode,
                                              const float lastWidth,
                                              const YGMeasureMode lastHeightMode,
                                              const float lastHeight,
                                              const float lastComputedWidth,
                                              const float lastComputedHeight,
                                              const float marginRow,
                                              const float marginColumn);

WIN_EXPORT void YGNodeCopyStyle(const YGNodeRef dstNode, const YGNodeRef srcNode);

#define YG_NODE_PROPERTY(type, name, paramName)                          \
  WIN_EXPORT void YGNodeSet##name(const YGNodeRef node, type paramName); \
  WIN_EXPORT type YGNodeGet##name(const YGNodeRef node);

#define YG_NODE_STYLE_PROPERTY(type, name, paramName)                               \
  WIN_EXPORT void YGNodeStyleSet##name(const YGNodeRef node, const type paramName); \
  WIN_EXPORT type YGNodeStyleGet##name(const YGNodeRef node);

#define YG_NODE_STYLE_PROPERTY_UNIT(type, name, paramName)                                    \
  WIN_EXPORT void YGNodeStyleSet##name(const YGNodeRef node, const float paramName);          \
  WIN_EXPORT void YGNodeStyleSet##name##Percent(const YGNodeRef node, const float paramName); \
  WIN_EXPORT type YGNodeStyleGet##name(const YGNodeRef node);

#define YG_NODE_STYLE_PROPERTY_UNIT_AUTO(type, name, paramName) \
  YG_NODE_STYLE_PROPERTY_UNIT(type, name, paramName)            \
  WIN_EXPORT void YGNodeStyleSet##name##Auto(const YGNodeRef node);

#define YG_NODE_STYLE_EDGE_PROPERTY(type, name, paramName)    \
  WIN_EXPORT void YGNodeStyleSet##name(const YGNodeRef node,  \
                                       const YGEdge edge,     \
                                       const type paramName); \
  WIN_EXPORT type YGNodeStyleGet##name(const YGNodeRef node, const YGEdge edge);

#define YG_NODE_STYLE_EDGE_PROPERTY_UNIT(type, name, paramName)         \
  WIN_EXPORT void YGNodeStyleSet##name(const YGNodeRef node,            \
                                       const YGEdge edge,               \
                                       const float paramName);          \
  WIN_EXPORT void YGNodeStyleSet##name##Percent(const YGNodeRef node,   \
                                                const YGEdge edge,      \
                                                const float paramName); \
  WIN_EXPORT WIN_STRUCT(type) YGNodeStyleGet##name(const YGNodeRef node, const YGEdge edge);
