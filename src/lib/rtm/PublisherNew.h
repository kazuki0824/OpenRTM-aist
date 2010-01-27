// -*- C++ -*-
/*!
 * @file  PublisherNew.h
 * @brief PublisherNew class
 * @date  $Date: 2007-12-31 03:08:06 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006-2009
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#ifndef RTC_PUBLISHERNEW_H
#define RTC_PUBLISHERNEW_H

#include <coil/Task.h>
#include <coil/Mutex.h>
#include <coil/Condition.h>
#include <coil/PeriodicTask.h>

#include <rtm/RTC.h>
#include <rtm/PublisherBase.h>
#include <rtm/CdrBufferBase.h>
#include <rtm/DataPortStatus.h>
#include <rtm/SystemLogger.h>
#include <rtm/ConnectorBase.h>
#include <rtm/ConnectorListener.h>

namespace coil
{
  class Properties;
};

namespace RTC
{
  class InPortConsumer;
  /*!
   * @if jp
   * @class PublisherNew
   * @brief PublisherNew クラス
   *
   * バッファ内に新規データが格納されたタイミングで、その新規データを送信する。
   * データ送出タイミングを待つコンシューマを、送出する側とは異なるスレッドで
   * 動作させる場合に使用。
   * Publisherの駆動は、データ送出のタイミングになるまでブロックされ、
   * 送出タイミングの通知を受けると、即座にコンシューマの送出処理を呼び出す。
   *
   * @else
   * @class PublisherNew
   * @brief PublisherNew class
   *
   * Send new data at timing of when it is stored into the buffer.
   * This class is used when operating Consumer that waits for the data send
   * timing in different thread from one of the send side.
   * Publisher's driven is blocked until the data send timing reaches, if the
   * send timing notification is received, the Consumer's send processing will
   * be invoked immediately.
   *
   * @endif
   */

  class PublisherNew
    : public PublisherBase
  {
  public:
    typedef coil::Mutex Mutex;
    typedef coil::Condition<Mutex> Condition;
    typedef coil::Guard<coil::Mutex> Guard;
    DATAPORTSTATUS_ENUM
    
    /*!
     * @if jp
     * @brief コンストラクタ
     *
     * コンストラクタ
     * 本 Publisher 用新規スレッドを生成する。
     *
     * @param consumer データ送出を待つコンシューマ
     * @param property 本Publisherの駆動制御情報を設定したPropertyオブジェクト
     *                 (本Publisherでは未使用)
     * @else
     * @brief Constructor
     *
     * Constructor
     * Create new thread for this Publisher.
     *
     * @param consumer Consumer that waits for the data sending
     * @param property Property object that is configured this Publisher's
     *                 control information.(Unused in this Publisher)
     * @endif
     */
    PublisherNew();
    
    /*!
     * @if jp
     * @brief デストラクタ
     *
     * デストラクタ
     *
     * @else
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~PublisherNew(void);

    /*!
     * @if jp
     * @brief 初期化
     * @else
     * @brief initialization
     * @endif
     */
    virtual ReturnCode init(coil::Properties& prop);
    /*!
     * @if jp
     * @brief InPortコンシューマのセット
     *
     * @else
     * @brief Store InPort consumer
     * @endif
     */
    virtual ReturnCode setConsumer(InPortConsumer* consumer);
    /*!
     * @if jp
     * @brief バッファのセット
     * @param buffer CDRバッファ
     * @return PORT_OK      正常終了
     *         INVALID_ARGS 不正な引数
     * @else
     * @brief Setting buffer pointer
     * @param buffer CDR buffer
     * @return PORT_OK      正常終了
     *         INVALID_ARGS 不正な引数
     * @endif
     */
    virtual ReturnCode setBuffer(CdrBufferBase* buffer);
    /*!
     * @if jp
     * @brief リスナを設定する。
     * @param info ConnectorInfo
     * @param listeners ConnectorListeners 
     * @return PORT_OK      正常終了
     *         INVALID_ARGS 不正な引数
     * @else
     * @brief Set the listener. 
     * @param info ConnectorInfo
     * @param listeners ConnectorListeners 
     * @return PORT_OK      Normal return
     *         INVALID_ARGS Invalid arguments
     * @endif
     */
    virtual ReturnCode setListener(ConnectorInfo& info,
                                   ConnectorListeners* listeners);
    /*!
     * @if jp
     * @brief データを書き込む
     * @param data 書き込むデータ 
     * @param sec タイムアウト時間
     * @param nsec タイムアウト時間
     * @return リターンコード
     * @else
     * @brief Write data 
     * @param data Data
     * @param sec Timeout period
     * @param nsec Timeout period
     * @return Return code
     * @endif
     */
    virtual ReturnCode write(const cdrMemoryStream& data,
                             unsigned long sec,
                             unsigned long usec);
    /*!
     * @if jp
     *
     * @brief アクティブ化確認
     * 
     * アクティブ化されているか確認する。
     *
     * @return 状態確認結果(アクティブ状態:true、非アクティブ状態:false)
     *
     * @else
     *
     * @brief Confirm to activate
     * 
     * Confirm that has been activated.
     *
     * @return Result of state confirmation
     *         (Active state:true, Inactive state:false)
     *
     * @endif
     */
    virtual bool isActive();
    /*!
     * @if jp
     * @brief アクティブ化
     * @return リターンコード
     *
     * @else
     *
     * @brief activation
     * @return Return code
     *
     * @endif
     */
    virtual ReturnCode activate();
    /*!
     * @if jp
     * @brief 非アクティブ化
     * @return リターンコード
     *
     * @else
     *
     * @brief deactivation
     * @return Return code
     *
     * @endif
     */
    virtual ReturnCode deactivate();
    
    /*!
     * @if jp
     * @brief スレッド実行関数
     *
     * ACE_Task::svc() のオーバーライド
     * バッファ内のデータが更新されるまでスレッドを待機させる。
     *
     * @return 実行結果
     *
     * @else
     * @brief Thread execution function
     *
     * ACE_Task::svc() override function.
     * Make thread stand by until data in the buffer is updated.
     *
     * @return Execution result
     *
     * @endif
     */
    virtual int svc(void);
    
    /*!
     * @if jp
     * @brief タスク開始
     *
     * ACE_Task::open() のオーバーライド
     * 本 publisher 用新規スレッドを生成する。
     *
     * @param args スレッド生成用引数(本Publisherでは未使用)
     *
     * @return 実行結果
     *
     * @else
     * @brief Task start function
     *
     * ACE_Task::open() override function.
     * Create newly thread for this Publisher.
     *
     * @param args Thread creation arguments (Unused in this Publisher)
     *
     * @return Execution result
     *
     * @endif
     */
    //    virtual int open(void *args);
    
    /*!
     * @if jp
     * @brief タスク終了関数
     *
     * ACE_Task::release() のオーバーライド
     * 駆動フラグをfalseに設定し、本 Publisher の駆動を停止する。
     * ただし、駆動スレッドがブロックされている場合には、
     * 最大１回コンシューマの送出処理が呼び出される場合がある。
     *
     * @else
     * @brief Task terminate function
     *
     * ACE_Task::release() override function.
     * Set 2driven flag to false, and terminate this Publisher's operation.
     * However, if the driven thread is blocked, Consumer's send
     * processing may be invoked maximum once.
     *
     * @endif
     */
    //    virtual void release();
    
  protected:
    enum Policy
      {
        ALL,
        FIFO,
        SKIP,
        NEW
      };

    /*!
     * @if jp
     * @brief PushPolicy の設定
     * @else
     * @brief Setting PushPolicy
     * @endif
     */
    void setPushPolicy(const coil::Properties& prop);

    /*!
     * @if jp
     * @brief Task の設定
     * @else
     * @brief Setting Task
     * @endif
     */
    bool createTask(const coil::Properties& prop);

    /*!
     * @brief push "all" policy
     */
    ReturnCode pushAll();

    /*!
     * @brief push "fifo" policy
     */
    ReturnCode pushFifo();

    /*!
     * @brief push "skip" policy
     */
    ReturnCode pushSkip();

    /*!
     * @brief push "new" policy
     */
    ReturnCode pushNew();

    /*!
     * @brief return code conversion (BufferStatus -> DataPortStatus)
     *
     *
     */
    ReturnCode convertReturn(BufferStatus::Enum status,
                             const cdrMemoryStream& data);

    /*!
     * @if jp
     * @brief DataPortStatusに従ってリスナへ通知する関数を呼び出す。
     * @param status DataPortStatus
     * @param data cdrMemoryStream
     * @return リターンコード
     *
     * @else
     * @brief This method calls the function notified the listener 
     *        according to DataPortStatus.  
     * @param status DataPortStatus
     * @param data cdrMemoryStream
     * @return Return code
     *
     * @endif
     */
    ReturnCode invokeListener(DataPortStatus::Enum status,
                              const cdrMemoryStream& data);
    
    /*!
     * @brief Connector data listener functions
     */
    /*!
     * @if jp
     * @brief ON_BUFFER_WRITEのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_BUFFER_WRITE. 
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferWrite(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_WRITE].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_BUFFER_FULLのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_BUFFER_FULL. 
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferFull(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_FULL].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_BUFFER_WRITE_TIMEOUTのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_BUFFER_WRITE_TIMEOUT. 
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferWriteTimeout(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_WRITE_TIMEOUT].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_BUFFER_OVERWRITEのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_BUFFER_OVERWRITE.
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferWriteOverwrite(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_OVERWRITE].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_BUFFER_READのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_BUFFER_READ.
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onBufferRead(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_BUFFER_READ].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_SENDのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_SEND. 
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onSend(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_SEND].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVEDのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_RECEIVED. 
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onReceived(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVED].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVER_FULLのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_RECEIVER_FULL. 
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onReceiverFull(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVER_FULL].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVER_TIMEOUTのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_RECEIVER_TIMEOUT. 
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onReceiverTimeout(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVER_TIMEOUT].notify(m_profile, data);
    }

    /*!
     * @if jp
     * @brief ON_RECEIVER_ERRORのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_RECEIVER_ERROR.
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onReceiverError(const cdrMemoryStream& data)
    {
      m_listeners->
        connectorData_[ON_RECEIVER_ERROR].notify(m_profile, data);
    }

    /*!
     * @brief Connector listener functions
     */
//    inline void onBufferEmpty()
//    {
//      m_listeners->
//        connector_[ON_BUFFER_EMPTY].notify(m_profile);
//    }

//    inline void onBufferReadTimeout()
//    {
//      m_listeners->
//        connector_[ON_BUFFER_READ_TIMEOUT].notify(m_profile);
//    }

//    inline void onSenderEmpty()
//    {
//      m_listeners->
//        connector_[ON_SENDER_EMPTY].notify(m_profile);
//    }

//    inline void onSenderTimeout()
//    {
//      m_listeners->
//        connector_[ON_SENDER_TIMEOUT].notify(m_profile);
//    }

    /*!
     * @if jp
     * @brief ON_SENDER_ERRORのリスナへ通知する。 
     * @param data cdrMemoryStream
     * @else
     * @brief This method is notified to listeners of ON_SENDER_ERROR.
     * @param data cdrMemoryStream
     * @endif
     */
    inline void onSenderError()
    {
      m_listeners->
        connector_[ON_SENDER_ERROR].notify(m_profile);
    }


  private:
    Logger rtclog;
    InPortConsumer* m_consumer;
    CdrBufferBase* m_buffer;
    ConnectorInfo m_profile;
    coil::PeriodicTaskBase* m_task;
    ConnectorListeners* m_listeners;
    ReturnCode m_retcode;
    Mutex m_retmutex;
    Policy m_pushPolicy;
    int m_skipn;
    bool m_active;
    int m_leftskip;
  };
};     // namespace RTC

extern "C"
{
  void DLL_EXPORT PublisherNewInit();
};

#endif // RTC_PUBLISHERNEW_H

