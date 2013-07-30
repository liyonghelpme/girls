#include "HelloWorldScene.h"
#include "AppMacros.h"
#include <stdlib.h>
USING_NS_CC;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    	
    CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage("pic1.jpg");
    CCSprite *sp1 = CCSprite::createWithTexture(tex);
    sp1->setPosition(ccp(300, 200));
    addChild(sp1);

    t2 = CCTextureCache::sharedTextureCache()->addImage("pic2.png");
    sp2 = CCSprite::createWithTexture(t2);
    sp2->setPosition(ccp(300, 200));
    addChild(sp2);

    //参照CCTexture2D initWithData 写入数据时候的格式
    //读取数据的时候的格式
    glPixelStorei(GL_PACK_ALIGNMENT, 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t2->getName());

    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glPixelStorei(GL_PACK_ROW_LENGTH, t2->getPixelsWide());
    data = (unsigned char *)malloc(4*t2->getPixelsWide()*t2->getPixelsHigh());
    GLint inf;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &inf);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    CCLog("internal format %d %d", inf, GL_RGBA);
    int dw = t2->getPixelsWide();
    int dh = t2->getPixelsHigh();

    CCTexture2D *temp = new CCTexture2D();
    temp->initWithData(data, kCCTexture2DPixelFormat_RGBA8888, dw, dh, CCSizeMake(dw, dh));
    CCSprite *sp3 = CCSprite::createWithTexture(temp);
    sp3->setPosition(ccp(50, 50));
    addChild(sp3);


    /*
    for(int i = 0; i < dw; i++) {
        for(int j =0; j < 1; j++) {
            //CCLog("data %d", data[j*dw*4+i*4]);
            data[j*dw*4+i*4+3] = 255;
            data[j*dw*4+i*4+3] = 255;
            data[j*dw*4+i*4+3] = 255;
            data[j*dw*4+i*4+3] = 255;
        }
    }
    */



    circle = CCTextureCache::sharedTextureCache()->addImage("circle.png");
    cirData = (unsigned char*)malloc(4*circle->getPixelsWide()*circle->getPixelsHigh());

    glPixelStorei(GL_PACK_ROW_LENGTH, circle->getPixelsWide());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, circle->getName());
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, cirData);
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    CCLog("init Hello World Scene");


    int cw = circle->getPixelsWide();
    int ch = circle->getPixelsHigh();
    /*
    for(int i = 0; i < cw; i++) {
        for(int j = 0; j < ch; j++) {
            cirData[j*cw*4+i*4] = 255;
            cirData[j*cw*4+i*4+1] = 255;
            cirData[j*cw*4+i*4+2] = 255;
            cirData[j*cw*4+i*4+3] = 255;
        }
    }
    */


    CCTexture2D *temp2 = new CCTexture2D();
    temp2->initWithData(cirData, kCCTexture2DPixelFormat_RGBA8888, cw, ch, CCSizeMake(cw, ch));
    CCSprite *sp4 = CCSprite::createWithTexture(temp2);
    sp4->setPosition(ccp(80, 50));
    addChild(sp4);

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


bool HelloWorld::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {

    CCPoint p = pTouch->getLocation();
    p = sp2->convertToNodeSpace(p);

    CCLog("touchPoint %f %f", p.x, p.y);

    int wide = circle->getPixelsWide();
    wide -= wide%2;
    int high = circle->getPixelsHigh();
    high -= high%2;
    CCLog("circle size %d %d", wide, high);

    int dw = t2->getPixelsWide();
    int dh = t2->getPixelsHigh();
    CCLog("data width height %d %d", dw, dh);


    int left = int(p.x)-wide/2;
    int right = int(p.x)+wide/2;
    int bottom = int(p.y)-high/2;
    int top = int(p.y)+high/2;

    CCLog("touchBegan %d %d %d %d", left, right, bottom, top);
    
    //绘制防止超出边界
    left = (std::max)(0, left);
    right = (std::min)(dw, right);
    bottom = (std::max)(0, bottom);
    top = (std::min)(dh, top);
    CCLog("adjust touchBegan %d %d %d %d", left, right, bottom, top);



    /*
    for(int i = 0; i < right; i++) {
        for(int j = 0; j < top; j++) {
            int drow = bottom+j;
            int dcol = left+i;
            int dIndex = drow*dw*4+dcol*4+3;
            int cirIndex = j*wide*4+i*4;
            //alpha value  -=  red color
            int temp = data[dIndex]-cirData[cirIndex];
            temp = (std::max)(temp, 0);
            data[dIndex] = temp;
        }
    }
    */
    
    for(int i = left ; i < right; i++) {
        for(int j = bottom; j < top; j++) {
            int drow = bottom+j;
            drow = dh-drow;
            int dcol = left+i;
            int dIndex = drow*dw*4+dcol*4+0;
            int cirIndex = (j-bottom)*circle->getPixelsWide()*4+(i-left)*4;
            int temp = int(data[dIndex+3])-int(cirData[cirIndex]);
            data[dIndex+3] = (std::max)(temp, 0); 
            /*
            data[dIndex] = 255;
            data[dIndex+1] = 0;
            data[dIndex+2] = 0;
            data[dIndex+3] = 255;
            */
        }
    }
    
    
    //int offset = bottom*dw*4+left*4;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t2->getName());
    //glTexSubImage2D(GL_TEXTURE_2D, 0, left, bottom, wide, high, GL_RGBA, GL_UNSIGNED_BYTE, data+)
    //参考 CCTexture2d 写入数据
    unsigned int bytesPerRow = dw*32/8;
    if(bytesPerRow % 8 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    }
    else if(bytesPerRow % 4 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else if(bytesPerRow % 2 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, dw, dh, GL_RGBA, GL_UNSIGNED_BYTE, data);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)dw, (GLsizei)dh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return true;
}
void HelloWorld::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
}
void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
}
