//
// Copyright (C) 2004-2008 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOCI_H_INCLUDED
#define SOCI_H_INCLUDED

#ifdef _MSC_VER
#pragma warning(disable:4251 4512 4511)
#endif

// namespace soci
#include "soci_backend-loader.h"
#include "soci_blob.h"
#include "soci_blob-exchange.h"
#include "soci_connection-pool.h"
#include "soci_error.h"
#include "soci_exchange-traits.h"
#include "soci_into.h"
#include "soci_into-type.h"
#include "soci_once-temp-type.h"
#include "soci_prepare-temp-type.h"
#include "soci_procedure.h"
#include "soci_ref-counted-prepare-info.h"
#include "soci_ref-counted-statement.h"
#include "soci_row.h"
#include "soci_row-exchange.h"
#include "soci_rowid.h"
#include "soci_rowid-exchange.h"
#include "soci_rowset.h"
#include "soci_session.h"
#include "soci_soci-backend.h"
#include "soci_soci-config.h"
#include "soci_soci-platform.h"
#include "soci_statement.h"
#include "soci_transaction.h"
#include "soci_type-conversion.h"
#include "soci_type-conversion-traits.h"
#include "soci_type-holder.h"
#include "soci_type-ptr.h"
#include "soci_unsigned-types.h"
#include "soci_use.h"
#include "soci_use-type.h"
#include "soci_values.h"
#include "soci_values-exchange.h"

// namespace boost
#ifdef SOCI_USE_BOOST
#include <boost/version.hpp>
#if defined(BOOST_VERSION) && BOOST_VERSION >= 103500
#include "boost-fusion.h"
#endif // BOOST_VERSION
#include "boost-optional.h"
#include "boost-tuple.h"
#include "boost-gregorian-date.h"
#endif // SOCI_USE_BOOST

#endif // SOCI_H_INCLUDED
