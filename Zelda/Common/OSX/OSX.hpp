/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

/*
 -----------------------------------------------------------------------------
 This source file is part of OGRE
 (Object-oriented Graphics Rendering Engine)
 For the latest info, see http://www.ogre3d.org/
 
 Copyright (c) 2000-2013 Torus Knot Software Ltd
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#ifndef _OSX_HPP_
#define _OSX_HPP_

#include "OgrePlatform.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
#error This header is for use with Mac OS X only
#endif

#ifdef __OBJC__

#define USE_DISPLAYLINK 0

#import "OgreOSXCocoaWindow.h"
#import <QuartzCore/CVDisplayLink.h>


#include "../Config/TypeDefines.hpp"
#include GAME_CLASS_HEADER

using namespace Ogre;

// All this does is suppress some messages in the run log.  NSApplication does not
// implement buttonPressed and apps without a NIB have no target for the action.
@implementation NSApplication (_suppressUnimplementedActionWarning)
- (void) buttonPressed:(id)sender
{
    /* Do nothing */
}
@end


#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface AppDelegate : NSObject <NSApplicationDelegate>
#else
@interface AppDelegate : NSObject
#endif
{
    NSTimer *mTimer;
    
    NSDate *mDate;
    NSTimeInterval mLastFrameTime;
    CVDisplayLinkRef mDisplayLink; //display link for managing rendering thread
}

- (void)go;
- (void)renderOneFrame:(id)sender;
- (void)shutdown;

@property (retain, atomic) NSTimer *mTimer;
@property (nonatomic) NSTimeInterval mLastFrameTime;

@end

static CGame *pGame = 0;

#if __LP64__
static id mAppDelegate;

// DisplayLink callback
#if USE_DISPLAYLINK
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime,
                                      CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    if(!sb.mIsShuttingDown &&
       Ogre::Root::getSingletonPtr() &&
       Ogre::Root::getSingleton().isInitialised())
    {
        NSOpenGLContext *ctx = static_cast<OSXCocoaWindow *>(sb.mWindow)->nsopenGLContext();
        CGLContextObj cglContext = (CGLContextObj)[ctx CGLContextObj];
        
        // Lock the context before we render into it.
        CGLLockContext(cglContext);
        
        // Calculate the time since we last rendered.
        Real deltaTime = 1.0 / (outputTime->rateScalar * (Real)outputTime->videoTimeScale / (Real)outputTime->videoRefreshPeriod);
        
        // Make the context current and dispatch the render.
        [ctx makeCurrentContext];
        dispatch_async(dispatch_get_main_queue(), ^(void)
                       {
                           Ogre::Root::getSingleton().renderOneFrame(deltaTime);
                       });
        
        CGLUnlockContext(cglContext);
    }
    else if(sb.mIsShuttingDown)
    {
        [(AppDelegate *)mAppDelegate shutdown];
    }
    return kCVReturnSuccess;
}
#endif
#endif

@implementation AppDelegate

@synthesize mTimer;
@dynamic mLastFrameTime;

- (NSTimeInterval)mLastFrameTime
{
    return mLastFrameTime;
}

- (void)setLastFrameTime:(NSTimeInterval)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        mLastFrameTime = frameInterval;
    }
}

- (void)go {
    pGame = CGame::getSingletonPtr();
    
    if (!pGame)
        pGame = new CGame();
        
    CGame &game(*pGame);
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mTimer = nil;
    
    try {
        game.go();
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        
        // Clear event times
		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
        }
#if __LP64__ && USE_DISPLAYLINK
        CVReturn ret = kCVReturnSuccess;
        // Create a display link capable of being used with all active displays
        ret = CVDisplayLinkCreateWithActiveCGDisplays(&mDisplayLink);
        
        // Set the renderer output callback function
        ret = CVDisplayLinkSetOutputCallback(mDisplayLink, &MyDisplayLinkCallback, self);
        
        // Set the display link for the current renderer
        NSOpenGLContext *ctx = static_cast<OSXCocoaWindow *>(sb.mWindow)->nsopenGLContext();
        NSOpenGLPixelFormat *fmt = static_cast<OSXCocoaWindow *>(sb.mWindow)->nsopenGLPixelFormat();
        CGLContextObj cglContext = (CGLContextObj)[ctx CGLContextObj];
        CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[fmt CGLPixelFormatObj];
        ret = CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(mDisplayLink, cglContext, cglPixelFormat);
        
        // Activate the display link
        ret = CVDisplayLinkStart(mDisplayLink);
#else
        mTimer = [[NSTimer timerWithTimeInterval: 0.001 target:self selector:@selector(renderOneFrame:) userInfo:self repeats:true] retain];
        [[NSRunLoop currentRunLoop] addTimer:mTimer forMode: NSDefaultRunLoopMode];
        [[NSRunLoop currentRunLoop] addTimer:mTimer forMode: NSEventTrackingRunLoopMode]; // Ensure timer fires during resize
#endif
        [pool release];
        }
        
        - (void)applicationDidFinishLaunching:(NSNotification *)application {
            mLastFrameTime = 1;
            mTimer = nil;
            
            [self go];
        }
        
        - (void)shutdown {
            if(mDisplayLink)
            {
                CVDisplayLinkStop(mDisplayLink);
                CVDisplayLinkRelease(mDisplayLink);
                mDisplayLink = nil;
            }
            
            [NSApp terminate:nil];
        }
        
        - (void)renderOneFrame:(id)sender
        {
            if(Ogre::Root::getSingletonPtr() &&
               Ogre::Root::getSingleton().isInitialised() && !Ogre::Root::getSingleton().endRenderingQueued())
            {
                Ogre::Root::getSingleton().renderOneFrame();
            }
            else            {
                if(mTimer)
                {
                    [mTimer invalidate];
                    mTimer = nil;
                }
                
                [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
            }
        }
        
        @end
        
#endif
        
#endif
