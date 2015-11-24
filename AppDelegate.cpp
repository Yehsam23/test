#include "AppDelegate.h"

#include "cocos2d.h"

#include "SimpleAudioEngine.h"
#include "audio/include/AudioEngine.h"

#include "Scene/LaunchScene/LaunchScene.h"

#include "ToolBox/CodeUtility.h"
#include "AudioBox/MusicUtility.h"
#include "GameGlobal/GameGlobal.h"
#include "GameSetting.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace experimental;

static Size designResolutionSize = Size(512, 288);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    GLContextAttrs glContextAttrs = {5, 6, 5, 0, 16, 8};
#else
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
#endif
    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("MJ2");
        director->setOpenGLView(glview);
    }
    
    glview->setMultipleTouchEnabled(false);                 //關閉Mutilple Touch
    FileUtils::getInstance()->setUseFileEncrypt(false);     //不使用圖片加密
    
    Size deviceSize = glview->getFrameSize();
    std::vector<std::string> resDirOrders;
    std::vector<std::string> langDirOrders;             //語言順序
    
    std::string szShopPathString = FileUtils::getInstance()->getWritablePath() + "shop/";
    std::string szEmotionsPathString = FileUtils::getInstance()->getWritablePath() + "emotions/";
    
    if(CodeUtility::deviceIsApplePad() || (deviceSize.height / deviceSize.width) > 2.0f/3.0f)     //是iPad要改解希度 或 Android手機比例大於2:3的比例
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        designResolutionSize = Size(512, 324);
#else
        //要把長設到432
        float ratio = (432.0f / deviceSize.width);
        designResolutionSize = Size(512, deviceSize.height * ratio);
#endif
    }
    
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    
    //解析度大小
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    if (deviceSize.height > 768 && CodeUtility::deviceIsApplePad())                         //iPad Retain才用ipadhd
    {
        resDirOrders.push_back(largeResource.directory);
        szShopPathString += largeResource.directory;
        director->setContentScaleFactor(largeResource.size.width / designResolutionSize.width);
        szEmotionsPathString += largeResource.directory;
        
    }
    else if(deviceSize.height > 320)                     //iPhone Retain
    {
        resDirOrders.push_back(mediumResource.directory);
        szShopPathString += mediumResource.directory;
        director->setContentScaleFactor(mediumResource.size.width / designResolutionSize.width);
        szEmotionsPathString += mediumResource.directory;
        
    }
    else
    {
        resDirOrders.push_back(smallResource.directory);
        szShopPathString += smallResource.directory;
        szEmotionsPathString += smallResource.directory;
        director->setContentScaleFactor(smallResource.size.width / designResolutionSize.width);
    }
#else
    int nTextureMaxSize = Configuration::getInstance()->getMaxTextureSize();
    int memorySize = CodeUtility::getTotalMemory();
    
    if (deviceSize.width > mediumResource.size.width && nTextureMaxSize >= 4096 && 0)        //先不包HDR
    {
        resDirOrders.push_back(largeResource.directory);
        szShopPathString += largeResource.directory;
        szEmotionsPathString += largeResource.directory;
        
        director->setContentScaleFactor(largeResource.size.width / designResolutionSize.width);
    }
    else if (deviceSize.height > 320 && nTextureMaxSize >= 2048 && memorySize > 230)
    {
        resDirOrders.push_back(mediumResource.directory);
        szShopPathString += mediumResource.directory;
        szEmotionsPathString += mediumResource.directory;
        director->setContentScaleFactor(mediumResource.size.width / designResolutionSize.width);
    }
    else
    {
        resDirOrders.push_back(smallResource.directory);
        szShopPathString += smallResource.directory;
        szEmotionsPathString += smallResource.directory;
        director->setContentScaleFactor(smallResource.size.width / designResolutionSize.width);
    }
#endif
    
    //語言包
    langDirOrders.push_back("zh_Hant");
    
    //設定現在用得Resource解希度目錄
    CodeUtility::setResourceFolder(resDirOrders[0]);
    
    //加入下載的地方
    resDirOrders.push_back(szShopPathString);
    resDirOrders.push_back(szEmotionsPathString);
    
    FileUtils::getInstance()->setSearchPaths(resDirOrders);
    if(langDirOrders.size())
        FileUtils::getInstance()->setSearchResolutionsOrder(langDirOrders);
    
#if DEBUG
    director->setDisplayStats(false);
#else
    director->setDisplayStats(false);
#endif
    
    director->setAnimationInterval(1.0 / 60);
    
    director->setProjection(Director::Projection::_2D);
    director->setDepthTest(false);                              //這樣大區塊的Texture移動才不會閃 SuperAnimNode也不會有問題 或許會照成別的問題
    
    CodeUtility::setScreenBonus(Size(432, 288));                //設定現在畫面的大小比設計的大小差多少
    if(CodeUtility::deviceIsApplePad())
    {
        Size winSize = Director::getInstance()->getVisibleSize();
        CodeUtility::setApplePadOffset(fabsf(winSize.height - 576)/2.0f, 0);
    }
    
    GameGlobal::initDataModal();                                //產生DataModal 太快產生 會有問題
    
    //設定是否用新音效引擎
#if USE_NEW_AUDIO_ENGINE
    bool useNewAudioEngine = UserDefault::getInstance()->getBoolForKey("UseNewAudioEngine", true);
    MusicUtility::setUseNewAudioEngine(useNewAudioEngine);
#endif
    
    auto scene = LaunchScene::scene();
    
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();
    MusicUtility::applicationDidEnterBackground();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AnalyticX::flurryEndSession();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    Director::getInstance()->resume();
    MusicUtility::applicationWillEnterForeground();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AnalyticX::flurryStartSession(FLURRY_ID);
#endif
}
