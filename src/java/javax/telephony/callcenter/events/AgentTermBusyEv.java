/*
#pragma ident "%W%      %E%     SMI"

 * Copyright (c) 1996 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for NON-COMMERCIAL purposes and without
 * fee is hereby granted provided that this copyright notice
 * appears in all copies. Please refer to the file "copyright.html"
 * for further important copyright and licensing information.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF
 * THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR
 * ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
 */

package javax.telephony.callcenter.events;

/**
 * The <CODE>AgentTermBusyEv</CODE> interface indicates that an Agent has
 * moved into the <CODE>Agent.BUSY</CODE> state. This interface extends the
 * <CODE>AgentTermEv</CODE> interface and is reported via the
 * <CODE>AgentTerminalObserver</CODE> interface for the Terminal associated
 * with the Agent.
 * <p>
 * This interface defines no additional methods.
 * <p>
 * @see javax.telephony.Terminal
 * @see javax.telephony.TerminalObserver
 * @see javax.telephony.callcenter.Agent
 * @see javax.telephony.callcenter.AgentTerminalObserver
 * @see javax.telephony.callcenter.events.AgentTermEv
 * @deprecated as of JTAPI 1.4, replaced by {@link javax.telephony.callcenter.AgentTerminalEvent}
 */
public interface AgentTermBusyEv extends AgentTermEv {

  /**
   * Event id
   */
  public static final int ID = 308;
}
