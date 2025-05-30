// Copyright (c) 2020, Ryo Currency Project
// Portions copyright (c) 2014-2018, The Monero Project
//
// Portions of this file are available under BSD-3 license. Please see ORIGINAL-LICENSE for details
// All rights reserved.
//
// Authors and copyright holders give permission for following:
//
// 1. Redistribution and use in source and binary forms WITHOUT modification.
//
// 2. Modification of the source form for your own personal use.
//
// As long as the following conditions are met:
//
// 3. You must not distribute modified copies of the work to third parties. This includes
//    posting the work online, or hosting copies of the modified work for download.
//
// 4. Any derivative version of this work is also covered by this license, including point 8.
//
// 5. Neither the name of the copyright holders nor the names of the authors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// 6. You agree that this licence is governed by and shall be construed in accordance
//    with the laws of England and Wales.
//
// 7. You agree to submit all disputes arising out of or in connection with this licence
//    to the exclusive jurisdiction of the Courts of England and Wales.
//
// Authors and copyright holders agree that:
//
// 8. This licence expires and the work covered by it is released into the
//    public domain on 1st of February 2021
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "core_rpc_server_commands_defs.h"
#include "cryptonote_core/cryptonote_core.h"
#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "net/http_client.h"
#include "net/http_server_impl_base.h"
#include "p2p/net_node.h"

#include "common/gulps.hpp"

// yes, epee doesn't properly use its full namespace when calling its
// functions from macros.  *sigh*
using namespace epee;

namespace cryptonote
{
/************************************************************************/
/*                                                                      */
/************************************************************************/
class core_rpc_server : public epee::http_server_impl_base<core_rpc_server>
{
	GULPS_CAT_MAJOR("c_rpc_serv");
  public:
	static const command_line::arg_descriptor<std::string, false, true, 2> arg_rpc_bind_port;
	static const command_line::arg_descriptor<std::string> arg_rpc_restricted_bind_port;
	static const command_line::arg_descriptor<bool> arg_restricted_rpc;
	static const command_line::arg_descriptor<std::string> arg_bootstrap_daemon_address;
	static const command_line::arg_descriptor<std::string> arg_bootstrap_daemon_login;

	typedef epee::net_utils::connection_context_base connection_context;

	core_rpc_server(
		core &cr, nodetool::node_server<cryptonote::t_cryptonote_protocol_handler<cryptonote::core>> &p2p);

	static void init_options(boost::program_options::options_description &desc);
	bool init(
		const boost::program_options::variables_map &vm,
		const bool restricted,
		const network_type nettype,
		const std::string &port);
	network_type nettype() const { return m_nettype; }

	CHAIN_HTTP_TO_MAP2(connection_context); //forward http requests to uri map

	BEGIN_URI_MAP2()
	MAP_URI_AUTO_JON2("/get_height", on_get_height, COMMAND_RPC_GET_HEIGHT)
	MAP_URI_AUTO_JON2("/getheight", on_get_height, COMMAND_RPC_GET_HEIGHT)
	MAP_URI_AUTO_BIN2("/get_blocks.bin", on_get_blocks, COMMAND_RPC_GET_BLOCKS_FAST)
	MAP_URI_AUTO_BIN2("/getblocks.bin", on_get_blocks, COMMAND_RPC_GET_BLOCKS_FAST)
	MAP_URI_AUTO_BIN2("/get_blocks_by_height.bin", on_get_blocks_by_height, COMMAND_RPC_GET_BLOCKS_BY_HEIGHT)
	MAP_URI_AUTO_BIN2("/getblocks_by_height.bin", on_get_blocks_by_height, COMMAND_RPC_GET_BLOCKS_BY_HEIGHT)
	MAP_URI_AUTO_BIN2("/get_hashes.bin", on_get_hashes, COMMAND_RPC_GET_HASHES_FAST)
	MAP_URI_AUTO_BIN2("/gethashes.bin", on_get_hashes, COMMAND_RPC_GET_HASHES_FAST)
	MAP_URI_AUTO_BIN2("/get_o_indexes.bin", on_get_indexes, COMMAND_RPC_GET_TX_GLOBAL_OUTPUTS_INDEXES)
	MAP_URI_AUTO_BIN2("/get_random_outs.bin", on_get_random_outs, COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS)
	MAP_URI_AUTO_BIN2("/getrandom_outs.bin", on_get_random_outs, COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS)
	MAP_URI_AUTO_BIN2("/get_outs.bin", on_get_outs_bin, COMMAND_RPC_GET_OUTPUTS_BIN)
	MAP_URI_AUTO_BIN2("/get_random_rctouts.bin", on_get_random_rct_outs, COMMAND_RPC_GET_RANDOM_RCT_OUTPUTS)
	MAP_URI_AUTO_BIN2("/getrandom_rctouts.bin", on_get_random_rct_outs, COMMAND_RPC_GET_RANDOM_RCT_OUTPUTS)
	MAP_URI_AUTO_JON2("/get_transactions", on_get_transactions, COMMAND_RPC_GET_TRANSACTIONS)
	MAP_URI_AUTO_JON2("/gettransactions", on_get_transactions, COMMAND_RPC_GET_TRANSACTIONS)
	MAP_URI_AUTO_JON2("/get_alt_blocks_hashes", on_get_alt_blocks_hashes, COMMAND_RPC_GET_ALT_BLOCKS_HASHES)
	MAP_URI_AUTO_JON2("/is_key_image_spent", on_is_key_image_spent, COMMAND_RPC_IS_KEY_IMAGE_SPENT)
	MAP_URI_AUTO_JON2("/send_raw_transaction", on_send_raw_tx, COMMAND_RPC_SEND_RAW_TX)
	MAP_URI_AUTO_JON2("/sendrawtransaction", on_send_raw_tx, COMMAND_RPC_SEND_RAW_TX)
	MAP_URI_AUTO_JON2_IF("/start_mining", on_start_mining, COMMAND_RPC_START_MINING, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/stop_mining", on_stop_mining, COMMAND_RPC_STOP_MINING, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/mining_status", on_mining_status, COMMAND_RPC_MINING_STATUS, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/save_bc", on_save_bc, COMMAND_RPC_SAVE_BC, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/get_peer_list", on_get_peer_list, COMMAND_RPC_GET_PEER_LIST, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/set_log_hash_rate", on_set_log_hash_rate, COMMAND_RPC_SET_LOG_HASH_RATE, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/set_log_level", on_set_log_level, COMMAND_RPC_SET_LOG_LEVEL, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/set_log_categories", on_set_log_categories, COMMAND_RPC_SET_LOG_CATEGORIES, !m_restricted)
	MAP_URI_AUTO_JON2("/get_transaction_pool", on_get_transaction_pool, COMMAND_RPC_GET_TRANSACTION_POOL)
	MAP_URI_AUTO_JON2("/get_transaction_pool_hashes.bin", on_get_transaction_pool_hashes, COMMAND_RPC_GET_TRANSACTION_POOL_HASHES)
	MAP_URI_AUTO_JON2("/get_transaction_pool_stats", on_get_transaction_pool_stats, COMMAND_RPC_GET_TRANSACTION_POOL_STATS)
	MAP_URI_AUTO_JON2_IF("/stop_daemon", on_stop_daemon, COMMAND_RPC_STOP_DAEMON, !m_restricted)
	MAP_URI_AUTO_JON2("/get_info", on_get_info, COMMAND_RPC_GET_INFO)
	MAP_URI_AUTO_JON2("/getinfo", on_get_info, COMMAND_RPC_GET_INFO)
	MAP_URI_AUTO_JON2_IF("/get_net_stats", on_get_net_stats, COMMAND_RPC_GET_NET_STATS, !m_restricted)
	MAP_URI_AUTO_JON2("/get_limit", on_get_limit, COMMAND_RPC_GET_LIMIT)
	MAP_URI_AUTO_JON2_IF("/set_limit", on_set_limit, COMMAND_RPC_SET_LIMIT, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/out_peers", on_out_peers, COMMAND_RPC_OUT_PEERS, !m_restricted)
	MAP_URI_AUTO_JON2_IF("/in_peers", on_in_peers, COMMAND_RPC_IN_PEERS, !m_restricted)
	MAP_URI_AUTO_JON2("/get_outs", on_get_outs, COMMAND_RPC_GET_OUTPUTS)
	MAP_URI_AUTO_JON2_IF("/update", on_update, COMMAND_RPC_UPDATE, !m_restricted)
	BEGIN_JSON_RPC_MAP("/json_rpc")
	MAP_JON_RPC("get_block_count", on_getblockcount, COMMAND_RPC_GETBLOCKCOUNT)
	MAP_JON_RPC("getblockcount", on_getblockcount, COMMAND_RPC_GETBLOCKCOUNT)
	MAP_JON_RPC_WE("on_get_block_hash", on_getblockhash, COMMAND_RPC_GETBLOCKHASH)
	MAP_JON_RPC_WE("on_getblockhash", on_getblockhash, COMMAND_RPC_GETBLOCKHASH)
	MAP_JON_RPC_WE("get_block_template", on_getblocktemplate, COMMAND_RPC_GETBLOCKTEMPLATE)
	MAP_JON_RPC_WE("getblocktemplate", on_getblocktemplate, COMMAND_RPC_GETBLOCKTEMPLATE)
	MAP_JON_RPC_WE("submit_block", on_submitblock, COMMAND_RPC_SUBMITBLOCK)
	MAP_JON_RPC_WE("submitblock", on_submitblock, COMMAND_RPC_SUBMITBLOCK)
	MAP_JON_RPC_WE("get_last_block_header", on_get_last_block_header, COMMAND_RPC_GET_LAST_BLOCK_HEADER)
	MAP_JON_RPC_WE("getlastblockheader", on_get_last_block_header, COMMAND_RPC_GET_LAST_BLOCK_HEADER)
	MAP_JON_RPC_WE("get_block_header_by_hash", on_get_block_header_by_hash, COMMAND_RPC_GET_BLOCK_HEADER_BY_HASH)
	MAP_JON_RPC_WE("getblockheaderbyhash", on_get_block_header_by_hash, COMMAND_RPC_GET_BLOCK_HEADER_BY_HASH)
	MAP_JON_RPC_WE("get_block_header_by_height", on_get_block_header_by_height, COMMAND_RPC_GET_BLOCK_HEADER_BY_HEIGHT)
	MAP_JON_RPC_WE("getblockheaderbyheight", on_get_block_header_by_height, COMMAND_RPC_GET_BLOCK_HEADER_BY_HEIGHT)
	MAP_JON_RPC_WE("get_block_headers_range", on_get_block_headers_range, COMMAND_RPC_GET_BLOCK_HEADERS_RANGE)
	MAP_JON_RPC_WE("getblockheadersrange", on_get_block_headers_range, COMMAND_RPC_GET_BLOCK_HEADERS_RANGE)
	MAP_JON_RPC_WE("get_block", on_get_block, COMMAND_RPC_GET_BLOCK)
	MAP_JON_RPC_WE("getblock", on_get_block, COMMAND_RPC_GET_BLOCK)
	MAP_JON_RPC_WE_IF("get_connections", on_get_connections, COMMAND_RPC_GET_CONNECTIONS, !m_restricted)
	MAP_JON_RPC_WE("get_info", on_get_info_json, COMMAND_RPC_GET_INFO)
	MAP_JON_RPC_WE("hard_fork_info", on_hard_fork_info, COMMAND_RPC_HARD_FORK_INFO)
	MAP_JON_RPC_WE_IF("set_bans", on_set_bans, COMMAND_RPC_SETBANS, !m_restricted)
	MAP_JON_RPC_WE_IF("get_bans", on_get_bans, COMMAND_RPC_GETBANS, !m_restricted)
	MAP_JON_RPC_WE_IF("flush_txpool", on_flush_txpool, COMMAND_RPC_FLUSH_TRANSACTION_POOL, !m_restricted)
	MAP_JON_RPC_WE("get_output_histogram", on_get_output_histogram, COMMAND_RPC_GET_OUTPUT_HISTOGRAM)
	MAP_JON_RPC_WE("get_version", on_get_version, COMMAND_RPC_GET_VERSION)
	MAP_JON_RPC_WE_IF("get_coinbase_tx_sum", on_get_coinbase_tx_sum, COMMAND_RPC_GET_COINBASE_TX_SUM, !m_restricted)
	MAP_JON_RPC_WE_IF("get_alternate_chains", on_get_alternate_chains, COMMAND_RPC_GET_ALTERNATE_CHAINS, !m_restricted)
	MAP_JON_RPC_WE_IF("relay_tx", on_relay_tx, COMMAND_RPC_RELAY_TX, !m_restricted)
	MAP_JON_RPC_WE_IF("sync_info", on_sync_info, COMMAND_RPC_SYNC_INFO, !m_restricted)
	MAP_JON_RPC_WE("get_txpool_backlog", on_get_txpool_backlog, COMMAND_RPC_GET_TRANSACTION_POOL_BACKLOG)
	MAP_JON_RPC_WE("get_output_distribution", on_get_output_distribution, COMMAND_RPC_GET_OUTPUT_DISTRIBUTION)
	END_JSON_RPC_MAP()
	END_URI_MAP2()

	bool on_get_height(const COMMAND_RPC_GET_HEIGHT::request &req, COMMAND_RPC_GET_HEIGHT::response &res);
	bool on_get_blocks(const COMMAND_RPC_GET_BLOCKS_FAST::request &req, COMMAND_RPC_GET_BLOCKS_FAST::response &res);
	bool on_get_alt_blocks_hashes(const COMMAND_RPC_GET_ALT_BLOCKS_HASHES::request &req, COMMAND_RPC_GET_ALT_BLOCKS_HASHES::response &res);
	bool on_get_blocks_by_height(const COMMAND_RPC_GET_BLOCKS_BY_HEIGHT::request &req, COMMAND_RPC_GET_BLOCKS_BY_HEIGHT::response &res);
	bool on_get_hashes(const COMMAND_RPC_GET_HASHES_FAST::request &req, COMMAND_RPC_GET_HASHES_FAST::response &res);
	bool on_get_transactions(const COMMAND_RPC_GET_TRANSACTIONS::request &req, COMMAND_RPC_GET_TRANSACTIONS::response &res);
	bool on_is_key_image_spent(const COMMAND_RPC_IS_KEY_IMAGE_SPENT::request &req, COMMAND_RPC_IS_KEY_IMAGE_SPENT::response &res, bool request_has_rpc_origin = true);
	bool on_get_indexes(const COMMAND_RPC_GET_TX_GLOBAL_OUTPUTS_INDEXES::request &req, COMMAND_RPC_GET_TX_GLOBAL_OUTPUTS_INDEXES::response &res);
	bool on_send_raw_tx(const COMMAND_RPC_SEND_RAW_TX::request &req, COMMAND_RPC_SEND_RAW_TX::response &res);
	bool on_start_mining(const COMMAND_RPC_START_MINING::request &req, COMMAND_RPC_START_MINING::response &res);
	bool on_stop_mining(const COMMAND_RPC_STOP_MINING::request &req, COMMAND_RPC_STOP_MINING::response &res);
	bool on_mining_status(const COMMAND_RPC_MINING_STATUS::request &req, COMMAND_RPC_MINING_STATUS::response &res);
	bool on_get_random_outs(const COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS::request &req, COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS::response &res);
	bool on_get_outs_bin(const COMMAND_RPC_GET_OUTPUTS_BIN::request &req, COMMAND_RPC_GET_OUTPUTS_BIN::response &res);
	bool on_get_outs(const COMMAND_RPC_GET_OUTPUTS::request &req, COMMAND_RPC_GET_OUTPUTS::response &res);
	bool on_get_random_rct_outs(const COMMAND_RPC_GET_RANDOM_RCT_OUTPUTS::request &req, COMMAND_RPC_GET_RANDOM_RCT_OUTPUTS::response &res);
	bool on_get_info(const COMMAND_RPC_GET_INFO::request &req, COMMAND_RPC_GET_INFO::response &res);
	bool on_get_net_stats(const COMMAND_RPC_GET_NET_STATS::request& req, COMMAND_RPC_GET_NET_STATS::response& res);
	bool on_save_bc(const COMMAND_RPC_SAVE_BC::request &req, COMMAND_RPC_SAVE_BC::response &res);
	bool on_get_peer_list(const COMMAND_RPC_GET_PEER_LIST::request &req, COMMAND_RPC_GET_PEER_LIST::response &res);
	bool on_set_log_hash_rate(const COMMAND_RPC_SET_LOG_HASH_RATE::request &req, COMMAND_RPC_SET_LOG_HASH_RATE::response &res);
	bool on_set_log_level(const COMMAND_RPC_SET_LOG_LEVEL::request &req, COMMAND_RPC_SET_LOG_LEVEL::response &res);
	bool on_set_log_categories(const COMMAND_RPC_SET_LOG_CATEGORIES::request &req, COMMAND_RPC_SET_LOG_CATEGORIES::response &res);
	bool on_get_transaction_pool(const COMMAND_RPC_GET_TRANSACTION_POOL::request &req, COMMAND_RPC_GET_TRANSACTION_POOL::response &res, bool request_has_rpc_origin = true);
	bool on_get_transaction_pool_hashes(const COMMAND_RPC_GET_TRANSACTION_POOL_HASHES::request &req, COMMAND_RPC_GET_TRANSACTION_POOL_HASHES::response &res, bool request_has_rpc_origin = true);
	bool on_get_transaction_pool_stats(const COMMAND_RPC_GET_TRANSACTION_POOL_STATS::request &req, COMMAND_RPC_GET_TRANSACTION_POOL_STATS::response &res, bool request_has_rpc_origin = true);
	bool on_stop_daemon(const COMMAND_RPC_STOP_DAEMON::request &req, COMMAND_RPC_STOP_DAEMON::response &res);
	bool on_get_limit(const COMMAND_RPC_GET_LIMIT::request &req, COMMAND_RPC_GET_LIMIT::response &res);
	bool on_set_limit(const COMMAND_RPC_SET_LIMIT::request &req, COMMAND_RPC_SET_LIMIT::response &res);
	bool on_out_peers(const COMMAND_RPC_OUT_PEERS::request &req, COMMAND_RPC_OUT_PEERS::response &res);
	bool on_in_peers(const COMMAND_RPC_IN_PEERS::request &req, COMMAND_RPC_IN_PEERS::response &res);
	bool on_update(const COMMAND_RPC_UPDATE::request &req, COMMAND_RPC_UPDATE::response &res);

	//json_rpc
	bool on_getblockcount(const COMMAND_RPC_GETBLOCKCOUNT::request &req, COMMAND_RPC_GETBLOCKCOUNT::response &res);
	bool on_getblockhash(const COMMAND_RPC_GETBLOCKHASH::request &req, COMMAND_RPC_GETBLOCKHASH::response &res, epee::json_rpc::error &error_resp);
	bool on_getblocktemplate(const COMMAND_RPC_GETBLOCKTEMPLATE::request &req, COMMAND_RPC_GETBLOCKTEMPLATE::response &res, epee::json_rpc::error &error_resp);
	bool on_submitblock(const COMMAND_RPC_SUBMITBLOCK::request &req, COMMAND_RPC_SUBMITBLOCK::response &res, epee::json_rpc::error &error_resp);
	bool on_get_last_block_header(const COMMAND_RPC_GET_LAST_BLOCK_HEADER::request &req, COMMAND_RPC_GET_LAST_BLOCK_HEADER::response &res, epee::json_rpc::error &error_resp);
	bool on_get_block_header_by_hash(const COMMAND_RPC_GET_BLOCK_HEADER_BY_HASH::request &req, COMMAND_RPC_GET_BLOCK_HEADER_BY_HASH::response &res, epee::json_rpc::error &error_resp);
	bool on_get_block_header_by_height(const COMMAND_RPC_GET_BLOCK_HEADER_BY_HEIGHT::request &req, COMMAND_RPC_GET_BLOCK_HEADER_BY_HEIGHT::response &res, epee::json_rpc::error &error_resp);
	bool on_get_block_headers_range(const COMMAND_RPC_GET_BLOCK_HEADERS_RANGE::request &req, COMMAND_RPC_GET_BLOCK_HEADERS_RANGE::response &res, epee::json_rpc::error &error_resp);
	bool on_get_block(const COMMAND_RPC_GET_BLOCK::request &req, COMMAND_RPC_GET_BLOCK::response &res, epee::json_rpc::error &error_resp);
	bool on_get_connections(const COMMAND_RPC_GET_CONNECTIONS::request &req, COMMAND_RPC_GET_CONNECTIONS::response &res, epee::json_rpc::error &error_resp);
	bool on_get_info_json(const COMMAND_RPC_GET_INFO::request &req, COMMAND_RPC_GET_INFO::response &res, epee::json_rpc::error &error_resp);
	bool on_hard_fork_info(const COMMAND_RPC_HARD_FORK_INFO::request &req, COMMAND_RPC_HARD_FORK_INFO::response &res, epee::json_rpc::error &error_resp);
	bool on_set_bans(const COMMAND_RPC_SETBANS::request &req, COMMAND_RPC_SETBANS::response &res, epee::json_rpc::error &error_resp);
	bool on_get_bans(const COMMAND_RPC_GETBANS::request &req, COMMAND_RPC_GETBANS::response &res, epee::json_rpc::error &error_resp);
	bool on_flush_txpool(const COMMAND_RPC_FLUSH_TRANSACTION_POOL::request &req, COMMAND_RPC_FLUSH_TRANSACTION_POOL::response &res, epee::json_rpc::error &error_resp);
	bool on_get_output_histogram(const COMMAND_RPC_GET_OUTPUT_HISTOGRAM::request &req, COMMAND_RPC_GET_OUTPUT_HISTOGRAM::response &res, epee::json_rpc::error &error_resp);
	bool on_get_version(const COMMAND_RPC_GET_VERSION::request &req, COMMAND_RPC_GET_VERSION::response &res, epee::json_rpc::error &error_resp);
	bool on_get_coinbase_tx_sum(const COMMAND_RPC_GET_COINBASE_TX_SUM::request &req, COMMAND_RPC_GET_COINBASE_TX_SUM::response &res, epee::json_rpc::error &error_resp);
	bool on_get_alternate_chains(const COMMAND_RPC_GET_ALTERNATE_CHAINS::request &req, COMMAND_RPC_GET_ALTERNATE_CHAINS::response &res, epee::json_rpc::error &error_resp);
	bool on_relay_tx(const COMMAND_RPC_RELAY_TX::request &req, COMMAND_RPC_RELAY_TX::response &res, epee::json_rpc::error &error_resp);
	bool on_sync_info(const COMMAND_RPC_SYNC_INFO::request &req, COMMAND_RPC_SYNC_INFO::response &res, epee::json_rpc::error &error_resp);
	bool on_get_txpool_backlog(const COMMAND_RPC_GET_TRANSACTION_POOL_BACKLOG::request &req, COMMAND_RPC_GET_TRANSACTION_POOL_BACKLOG::response &res, epee::json_rpc::error &error_resp);
	bool on_get_output_distribution(const COMMAND_RPC_GET_OUTPUT_DISTRIBUTION::request &req, COMMAND_RPC_GET_OUTPUT_DISTRIBUTION::response &res, epee::json_rpc::error &error_resp);
	//-----------------------

  private:
	bool check_core_busy();
	bool check_core_ready();

	//utils
	uint64_t get_block_reward(const block &blk);
	bool fill_block_header_response(const block &blk, bool orphan_status, uint64_t height, const crypto::hash &hash, block_header_response &response);
	enum invoke_http_mode
	{
		JON,
		BIN,
		JON_RPC
	};
	template <typename COMMAND_TYPE>
	bool use_bootstrap_daemon_if_necessary(const invoke_http_mode &mode, const std::string &command_name, const typename COMMAND_TYPE::request &req, typename COMMAND_TYPE::response &res, bool &r);

	core &m_core;
	nodetool::node_server<cryptonote::t_cryptonote_protocol_handler<cryptonote::core>> &m_p2p;
	std::string m_bootstrap_daemon_address;
	epee::net_utils::http::http_simple_client m_http_client;
	boost::shared_mutex m_bootstrap_daemon_mutex;
	bool m_should_use_bootstrap_daemon;
	std::chrono::system_clock::time_point m_bootstrap_height_check_time;
	bool m_was_bootstrap_ever_used;
	network_type m_nettype;
	bool m_restricted;
};
}

BOOST_CLASS_VERSION(nodetool::node_server<cryptonote::t_cryptonote_protocol_handler<cryptonote::core>>, 1);
