//
// Licensed under the terms in License.txt
//
// Copyright 2010 Allen Ding. All rights reserved.
//

#import "KiwiConfiguration.h"

// This category is solely meant to coax Xcode into exposing the method names below during autocompletion.
// There is no implementation and this class definition must come before the macro definitions below.
@interface NSObject (KiwiVerifierMacroNames)

- (void)should;
- (void)shouldNot;
- (void)shouldBeNil;
- (void)shouldNotBeNil;
- (void)shouldEventually;
- (void)shouldNotEventually;
- (void)shouldEventuallyBeforeTimingOutAfter;
- (void)shouldNotEventuallyBeforeTimingOutAfter;

@end

#pragma mark -
#pragma mark Support Macros

#define KW_THIS_CALLSITE [KWCallSite callSiteWithFilename:@__FILE__ lineNumber:__LINE__]
#define KW_ADD_EXIST_VERIFIER(expectationType) [self addExistVerifierWithExpectationType:expectationType callSite:KW_THIS_CALLSITE]
#define KW_ADD_MATCH_VERIFIER(expectationType) [self addMatchVerifierWithExpectationType:expectationType callSite:KW_THIS_CALLSITE]
#define KW_ADD_ASYNC_VERIFIER(expectationType, timeOut) [self addAsyncVerifierWithExpectationType:expectationType callSite:KW_THIS_CALLSITE timeout:timeOut]

#pragma mark -
#pragma mark Keywords

// Kiwi macros used in specs for verifying expectations.
#define should attachToVerifier:KW_ADD_MATCH_VERIFIER(KWExpectationTypeShould) verifier:KW_ADD_EXIST_VERIFIER(KWExpectationTypeShould)
#define shouldNot attachToVerifier:KW_ADD_MATCH_VERIFIER(KWExpectationTypeShouldNot) verifier:KW_ADD_EXIST_VERIFIER(KWExpectationTypeShould)
#define shouldBeNil attachToVerifier:KW_ADD_EXIST_VERIFIER(KWExpectationTypeShouldNot)
#define shouldNotBeNil attachToVerifier:KW_ADD_EXIST_VERIFIER(KWExpectationTypeShould)
#define shouldEventually attachToVerifier:KW_ADD_ASYNC_VERIFIER(KWExpectationTypeShould, kKW_DEFAULT_PROBE_TIMEOUT)
#define shouldNotEventually attachToVerifier:KW_ADD_ASYNC_VERIFIER(KWExpectationTypeShouldNot, kKW_DEFAULT_PROBE_TIMEOUT)
#define shouldEventuallyBeforeTimingOutAfter(timeout) attachToVerifier:KW_ADD_ASYNC_VERIFIER(KWExpectationTypeShould, timeout)
#define shouldNotEventuallyBeforeTimingOutAfter(timeout) attachToVerifier:KW_ADD_ASYNC_VERIFIER(KWExpectationTypeShouldNot, timeout)

// used to wrap a pointer to an object that will change in the future (used with shouldEventually)
#define theObject(objectPtr) [KWFutureObject objectWithObjectPointer:objectPtr] // DEPRECATED
#define theReturnValueOfBlock(block) [KWFutureObject futureObjectWithBlock:block] // DEPRECATED
#define expectFutureValue(futureValue) [KWFutureObject futureObjectWithBlock:^{ return futureValue; }]

// used for message patterns to allow matching any value
#define any() [KWAny any]

// If a gcc compatible compiler is available, use the statement and
// declarations in expression extension to provide a convenient catch-all macro
// to create KWValues.
#if defined(__GNUC__)
    #define theValue(expr) \
        ({ \
            __typeof__(expr) kiwiReservedPrefix_lVar = expr; \
            [KWValue valueWithBytes:&kiwiReservedPrefix_lVar objCType:@encode(__typeof__(expr))]; \
        })
#endif // #if defined(__GNUC__)

// Allows for comparision of pointer values in expectations
#define thePointerValue(expr) [NSValue valueWithPointer:(expr)]

// Example group declarations.
#define SPEC_BEGIN(name) \
    \
    @interface name : KWSpec \
    \
    @end \
    \
    @implementation name \
    \
    + (void)buildExampleGroups { \

#define SPEC_END \
    } \
    \
    @end
