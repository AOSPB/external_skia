/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "Test.h"

#include "SkPathEffect.h"
#include "SkDashPathEffect.h"
#include "SkCornerPathEffect.h"

DEF_TEST(AsADashTest_noneDash, reporter) {
    SkAutoTUnref<SkCornerPathEffect> pe(SkCornerPathEffect::Create(1.0));
    SkPathEffect::DashInfo info;

    SkPathEffect::DashType dashType = pe->asADash(&info);
    REPORTER_ASSERT(reporter, SkPathEffect::kNone_DashType == dashType);
}

DEF_TEST(AsADashTest_nullInfo, reporter) {
    SkScalar inIntervals[] = { 4.0, 2.0, 1.0, 3.0 };
    const SkScalar phase = 2.0;
    SkAutoTUnref<SkDashPathEffect> pe(SkDashPathEffect::Create(inIntervals, 4, phase));

    SkPathEffect::DashType dashType = pe->asADash(NULL);
    REPORTER_ASSERT(reporter, SkPathEffect::kDash_DashType == dashType);
}

DEF_TEST(AsADashTest_usingDash, reporter) {
    SkScalar inIntervals[] = { 4.0, 2.0, 1.0, 3.0 };
    SkScalar totalIntSum = 10.0;
    const SkScalar phase = 2.0;

    SkAutoTUnref<SkDashPathEffect> pe(SkDashPathEffect::Create(inIntervals, 4, phase));

    SkPathEffect::DashInfo info;

    SkPathEffect::DashType dashType = pe->asADash(&info);
    REPORTER_ASSERT(reporter, SkPathEffect::kDash_DashType == dashType);
    REPORTER_ASSERT(reporter, 4 == info.fCount);
    REPORTER_ASSERT(reporter, SkScalarMod(phase, totalIntSum) == info.fPhase);

    // Since it is a kDash_DashType, allocate space for the intervals and recall asADash
    SkAutoTArray<SkScalar> intervals(info.fCount);
    info.fIntervals = intervals.get();
    pe->asADash(&info);
    REPORTER_ASSERT(reporter, inIntervals[0] == info.fIntervals[0]);
    REPORTER_ASSERT(reporter, inIntervals[1] == info.fIntervals[1]);
    REPORTER_ASSERT(reporter, inIntervals[2] == info.fIntervals[2]);
    REPORTER_ASSERT(reporter, inIntervals[3] == info.fIntervals[3]);

    // Make sure nothing else has changed on us
    REPORTER_ASSERT(reporter, 4 == info.fCount);
    REPORTER_ASSERT(reporter, SkScalarMod(phase, totalIntSum) == info.fPhase);
}