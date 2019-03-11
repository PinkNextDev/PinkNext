// Copyright (c) 2012-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_VERSION_H
#define BITCOIN_VERSION_H

/**
 * network protocol versioning
 */
// [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/version.h#L33
static const int PROTOCOL_VERSION = 60017;

//! initial proto version, to be increased after version/verack negotiation
static const int INIT_PROTO_VERSION = 209;

// [PINK] Do we need it?
//! In this version, 'getheaders' was introduced.
static const int GETHEADERS_VERSION = 31800;

//! disconnect from peers older than this proto version
static const int MIN_PEER_PROTO_VERSION = 60017;

// Allow connections from old peers, intended for temporary use when a mandatory update is pending.
// [PINK] https://github.com/Pink2Dev/Pink2/blob/2.2.3.0/src/version.h#L42
// [PINK] TODO: Use it.
static const int MIN_PEER_PROTO_VERSION_OLD = 60016;

//! nTime field added to CAddress, starting with this version;
//! if possible, avoid requesting addresses nodes older than this
static const int CADDR_TIME_VERSION = 31402;

//! BIP 0031, pong message, is enabled for all versions AFTER this one
static const int BIP0031_VERSION = 60000;

//! "filter*" commands are disabled without NODE_BLOOM after and including this version
static const int NO_BLOOM_VERSION = 60018;

//! "sendheaders" command and announcing blocks with headers starts with this version
static const int SENDHEADERS_VERSION = 60018;

//! "feefilter" tells peers to filter invs to you by fee starts with this version
static const int FEEFILTER_VERSION = 60018;

//! short-id-based block download starts with this version
static const int SHORT_IDS_BLOCKS_VERSION = 60018;

//! not banning for invalid compact blocks starts with this version
static const int INVALID_CB_NO_BAN_VERSION = 60018;

#endif // BITCOIN_VERSION_H
