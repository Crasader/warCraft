//
//  GameIapStore.h
//  appStoreKit
//
//  Created by wwl on 13-12-5.
//
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>
enum{
    IAP6=0,
    IAP30,
    IAP68,
    IAP198,
    IAP328,
    IAP648,
}buyCoinsTag;

@interface GameIapStore : NSObject <SKProductsRequestDelegate,SKPaymentTransactionObserver>
{
    const char* buyType;
    UIAlertView*alert;
    
    int serverId;
    int userID;
}

@property (nonatomic, assign) int _serverId;
@property (nonatomic, assign) int _userID;

-(id)init;
- (void) requestProUpgradeProductData;
-(void)RequestProductData;
-(bool)CanMakePay;
-(void)buy:(const char*)type;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction;
- (void) completeTransaction: (SKPaymentTransaction *)transaction;
- (void) failedTransaction: (SKPaymentTransaction *)transaction;
-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction;
-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error;
- (void) restoreTransaction: (SKPaymentTransaction *)transaction;
-(void)provideContent:(NSString *)product;
-(void)recordTransaction:(NSString *)product;
-(void)postData:(NSString*)data;

-(void)buyPaymentQueue:(BOOL) isYes;

//MM: 安全开关"等待中“alert窗口。
-(void)dismissWaitingAlert;

@end
