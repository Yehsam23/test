//
//  GameSetting.h
//  Poker13
//
//  Created by Yehsam on 13/4/18.
//
//

#ifndef _GameSetting_h
#define _GameSetting_h

#include "cocos2d.h"

#define INVITE_APP_NAME "神來也二人麻將"
#define GAME_COMPANY_ID_NATIVE 30    //遊戲的號碼

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    #if IS_HD
        #define FLURRY_ID "QCQ2M3899579NJP7HW2M"
        #define TAPJOY_ACTION_COMPLETE_ID "f7c657e9-f271-4f58-b549-12967830e34d"
    #else
        #define FLURRY_ID "BBF2PG37KRKDVVPHTH5B"
        #define TAPJOY_ACTION_COMPLETE_ID "dbfec983-c193-4feb-9962-009777702cff"
    #endif
#else
    #define FLURRY_ID "VZSVPS5T36QR7VBJ9CHV"
    #define TAPJOY_ACTION_COMPLETE_ID "6ad9e254-d2fc-4d50-a877-11b4455ee3b2"
#endif

#define SHOP_HAVE_MOTION 1          //商城是否有贏牌動畫

//廣告專區
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	#if IS_HD
		#define MEDIATION_AD_BANNER_320x50          ""
		#define MEDIATION_AD_BANNER_468x60          ""
		#define MEDIATION_AD_BANNER_728x90          ""
		#define MEDIATION_AD_BANNER_INTERSTITIAL    ""
	#else
		#define MEDIATION_AD_BANNER_320x50          ""
		#define MEDIATION_AD_BANNER_468x60          ""
		#define MEDIATION_AD_BANNER_728x90          ""
		#define MEDIATION_AD_BANNER_INTERSTITIAL    ""
	#endif
#else
	#define MEDIATION_AD_BANNER_320x50          ""
	#define MEDIATION_AD_BANNER_468x60          ""
	#define MEDIATION_AD_BANNER_728x90          ""
	#define MEDIATION_AD_BANNER_INTERSTITIAL    ""
#endif

#endif
