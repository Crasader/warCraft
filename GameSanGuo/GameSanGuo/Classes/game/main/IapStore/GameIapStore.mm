
//
//  GameIapStore.m
//  appStoreKit
//
//  Created by wwl on 13-12-5.
//
//

#import "GameIapStore.h"
#import "GtMBase64.h"
#include "SGPlayerInfo.h"
#include "GameConfig.h"
#include "cocos2d.h"
#include "SGMainManager.h"
//#include "SGMainScene.h"
//#include "SGStoreLayer.h"

#include "SGMsgId.h"
#include "ShopModule.pb.h"
#include "SGStringConfig.h"
#include "GlobalConfig.h"

#define ProductID_IAP6 @"com.sanguo.60"//6
#define ProductID_IAP30 @"com.sanguo.320" //30
#define ProductID_IAP68 @"com.sanguo.750" //68
#define ProductID_IAP198 @"com.sanguo.2300" //198
#define ProductID_IAP328 @"com.sanguo.3950" //328
#define ProductID_IAP648 @"com.sanguo.8100" //648

#define  ATTESTATION_HTTP "http://42.62.40.85:10088/SanguoPay/pay/apple/verifyReceipt"

@implementation GameIapStore
-(id)init
{
    if (self = [super init]) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        serverId = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
        userID = SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId();
        alert = NULL;
        
//        NSString *key = [NSString stringWithFormat:@"areaId%duserId%d", serverId, userID];
//        std::string data = CCUserDefault::sharedUserDefault()->getStringForKey([key UTF8String]);
//        if (data.length() > 0) {
//            NSString *data = [NSString stringWithUTF8String:[data UTF8String]];
//            [self postData:data];
//            alert = [[UIAlertView alloc]initWithTitle:@"请稍等"
//                                              message:@"正在为您请求购买。。。"
//                                             delegate:NULL
//                                    cancelButtonTitle:NULL
//                                    otherButtonTitles:NULL];
//            [alert show];
//        }
        
    }
    
    return self;
}
-(void)buy:(const char*)type
{
    
//    NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
//    if (transactions.count > 0) {
//        //检测是否有未完成的交易
//        SKPaymentTransaction* transaction = [transactions firstObject];
//        if (transaction.transactionState == SKPaymentTransactionStatePurchased) {
//            [self completeTransaction:transaction];
//            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
//            return;
//        }  
//    }

    buyType = type;
    if ([SKPaymentQueue canMakePayments]) {
        alert = [[UIAlertView alloc]initWithTitle:@str_Waiting
                                          message:@str_buy_requesting
                                         delegate:NULL
                                cancelButtonTitle:NULL
                                otherButtonTitles:NULL];
        [alert show];
        [self RequestProductData];
        NSLog(@"允许程序内付费购买");
    }
    else
    {
        NSLog(@"不允许程序内付费购买");
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@str_buy_permit
                                                           delegate:nil cancelButtonTitle:NSLocalizedString(@str_buy_close,nil) otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];
        
    }
}

-(bool)CanMakePay
{
    return [SKPaymentQueue canMakePayments];
}

-(void)RequestProductData
{
    NSLog(@"---------请求对应的产品信息------------");
    
    NSString *tyep = [NSString stringWithUTF8String:buyType];
    NSArray *product = [[NSArray alloc] initWithObjects:tyep,nil];
       
    NSSet *nsset = [NSSet setWithArray:product];
    SKProductsRequest *request=[[SKProductsRequest alloc] initWithProductIdentifiers: nsset];
    request.delegate=self;
    [request start];
    [product release];
}
//<SKProductsRequestDelegate> 请求协议
//收到的产品信息
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    
    NSLog(@"-----------收到产品反馈信息--------------");
    NSArray *myProduct = response.products;
    NSLog(@"产品Product ID:%@",response.invalidProductIdentifiers);
    NSLog(@"产品付费数量: %d", [myProduct count]);
    // populate UI
    for(SKProduct *product in myProduct){
        NSLog(@"product info");
        NSLog(@"SKProduct 描述信息%@", [product description]);
        NSLog(@"产品标题 %@" , product.localizedTitle);
        NSLog(@"产品描述信息: %@" , product.localizedDescription);
        NSLog(@"价格: %@" , product.price);
        NSLog(@"Product id: %@" , product.productIdentifier);
    }
    NSString *tyep = [NSString stringWithUTF8String:buyType];
    
    SKPayment *payment = [SKPayment paymentWithProductIdentifier:tyep];
        
    NSLog(@"---------发送购买请求------------");
    [[SKPaymentQueue defaultQueue] addPayment:payment];
    [request autorelease];
    
}
- (void)requestProUpgradeProductData
{
    NSLog(@"------请求升级数据---------");
    NSSet *productIdentifiers = [NSSet setWithObject:@"com.productid"];
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = self;
    [productsRequest start];
    
}
//弹出错误信息
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    [self dismissWaitingAlert];
    NSLog(@"-------弹出错误信息----------");
    UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",NULL) message:[error localizedDescription]
                                                       delegate:nil cancelButtonTitle:NSLocalizedString(@"Close",nil) otherButtonTitles:nil];
    [alerView show];
    [alerView release];
}

-(void) requestDidFinish:(SKRequest *)request
{
    NSLog(@"----------反馈信息结束--------------");
    
}

-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction{
    NSLog(@"-----PurchasedTransaction----");
    NSArray *transactions =[[NSArray alloc] initWithObjects:transaction, nil];
    [self paymentQueue:[SKPaymentQueue defaultQueue] updatedTransactions:transactions];
    [transactions release];
}

//<SKPaymentTransactionObserver> 千万不要忘记绑定，代码如下：
//----监听购买结果
//[[SKPaymentQueue defaultQueue] addTransactionObserver:self];

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions//交易结果
{
    NSLog(@"-----paymentQueue--------");
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased://交易完成
            {
                [self dismissWaitingAlert];
                [self completeTransaction:transaction];
                NSLog(@"-----交易完成 --------");
                NSLog(@"不允许程序内付费购买");
            }
                break;
            case SKPaymentTransactionStateFailed://交易失败
            {
                [self dismissWaitingAlert];
                [self failedTransaction:transaction];
                NSLog(@"-----交易失败 --------");
                UIAlertView *alerView2 =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                                     message:@str_buy_fail_retry
                                                                    delegate:nil cancelButtonTitle:NSLocalizedString(@str_buy_close,nil) otherButtonTitles:nil];
                
                [alerView2 show];
                [alerView2 release];

            }
                break;
            case SKPaymentTransactionStateRestored://已经购买过该商品
            {
                [self dismissWaitingAlert];
                [self restoreTransaction:transaction];
                NSLog(@"-----已经购买过该商品 --------");
            }
                break;
            case SKPaymentTransactionStatePurchasing:      //商品添加进列表
            {
                [self dismissWaitingAlert];
                NSLog(@"-----商品添加进列表 --------");
            }
                break;
            default:
                break;
        }
    }
}

- (void) completeTransaction: (SKPaymentTransaction *)transaction
{
    
    NSString *temptransactionReceipt  = [[NSString alloc] initWithData:[transaction transactionReceipt] encoding:NSUTF8StringEncoding];
    temptransactionReceipt = [[NSString alloc] initWithData:[GTMBase64 encodeData:[temptransactionReceipt dataUsingEncoding:NSUTF8StringEncoding]]
                                                   encoding:NSUTF8StringEncoding];;
    NSLog(@"服务器认证串: %@",temptransactionReceipt);
    
    //    [[temptransactionReceipt dataUsingEncoding:NSUTF8StringEncoding] setPostValue:[GTMBase64 stringByEncodingData:receipt] forKey:@"receipt"];
    
    NSString *data = [NSString stringWithFormat:@"%@",temptransactionReceipt];
//    std::string ss = [data UTF8String];
//    NSString *key = [NSString stringWithFormat:@"areaId%duserId%d", serverId, userID];
//    CCUserDefault::sharedUserDefault()->setStringForKey([key UTF8String], ss);
//    CCUserDefault::sharedUserDefault()->flush();
    /*
    [self postData:data];
    */
//    SGMainScene *sc = SGMainManager::shareMain()->getMainScene();
//    SGStoreLayer *stLayer = (SGStoreLayer*)sc->getChildByTag(sc->nowLayerTag);
    
#if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE) //台湾go2play ios
    SGMainManager::shareMain()->sendInvoiceRequst();
    SGMainManager::shareMain()->setBuyData([data UTF8String]);
#else //其他IOS计费回调，等待随时追加。
    main::ShopChargeRequest *request = new main::ShopChargeRequest;
    request->set_gameid(1);
    request->set_areaid(serverId);
    request->set_accountid(userID);
    request->set_roleid(SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
    request->set_receipt([data UTF8String]);
    SGSocketClient::sharedSocketClient()->send(MSG_STORE_BUY, request);
#endif

    
//    NSLog(@"-----completeTransaction--------");
//    // Your application should implement these two methods.
//    NSString *product = transaction.payment.productIdentifier;
//    if ([product length] > 0) {
//        
//        NSArray *tt = [product componentsSeparatedByString:@"."];
//        NSString *bookid = [tt lastObject];
//        if ([bookid length] > 0) {
//            [self recordTransaction:bookid];
//            [self provideContent:bookid];
//        }
//    }
    
    // Remove the transaction from the payment queue.
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
}

-(void)postData:(NSString*)data
{
    NSData *postData = [data dataUsingEncoding:NSUTF8StringEncoding];
    NSString *url = [NSString stringWithUTF8String:ATTESTATION_HTTP];
    NSMutableURLRequest *conn = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url]];
    [conn setHTTPMethod:@"POST"];
    [conn setTimeoutInterval:60.0f];
    [conn setCachePolicy:NSURLRequestUseProtocolCachePolicy];
    [conn setHTTPBody:postData];
    [NSURLConnection connectionWithRequest:conn delegate:self];//nahi
    
    //    NSData *returnData = [NSURLConnection sendSynchronousRequest:conn returningResponse:nil error:nil];//m hi
    //    NSString *s = [returnData description];
}

//记录交易
-(void)recordTransaction:(NSString *)product{
    NSLog(@"-----记录交易--------");
}

//处理下载内容
-(void)provideContent:(NSString *)product{
    NSLog(@"-----下载--------");
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction{
    NSLog(@"失败");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    
}
-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction{
    
}

- (void) restoreTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@" 交易恢复处理");
    
}

-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error{
    NSLog(@"-------paymentQueue----");
}


#pragma mark connection delegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSString *s = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    bool Yes = [s boolValue];
    if (Yes) {
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@str_buy_succeed
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@str_buy_close,nil)
                                                  otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];
    }else{
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@str_buy_fail
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@str_buy_close,nil)
                                                  otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];
    }
//    NSString *key = [NSString stringWithFormat:@"areaId%duserId%d", serverId, userID];
//    CCUserDefault::sharedUserDefault()->setStringForKey([key UTF8String], "");
//    CCUserDefault::sharedUserDefault()->flush();
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response{
    switch([(NSHTTPURLResponse *)response statusCode]) {
        case 200:
        case 206:
            break;
        case 304:
            break;
        case 400:
            break;
        case 404:
            break;
        case 416:
            break;
        case 403:
            break;
        case 401:
        case 500:
            break;
        default:
            break;
    }
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection{
    NSLog(@"connectionDidFinishLoading");
    [self dismissWaitingAlert];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    [self dismissWaitingAlert];
    UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                        message:@str_buy_back
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@str_buy_close,nil)
                                              otherButtonTitles:nil];
    [alerView show];
    [alerView release];
}


-(void)buyPaymentQueue:(BOOL)isYes
{
    [self dismissWaitingAlert];
    if (isYes) {
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@str_buy_succeed
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@str_buy_close,nil)
                                                  otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];
    }else{
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@str_buy_fail
                                                           delegate:nil
                                                  cancelButtonTitle:NSLocalizedString(@str_buy_close,nil)
                                                  otherButtonTitles:nil];
        
        [alerView show];
        [alerView release];
    }
}


-(void)dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

//MM: 安全开关"等待中“alert窗口。
-(void)dismissWaitingAlert
{
    if (alert != NULL)
    {
        [alert dismissWithClickedButtonIndex:0 animated:YES];
        alert = NULL;
    }
}

@end
