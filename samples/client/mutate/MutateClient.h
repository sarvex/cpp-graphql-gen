// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef MUTATECLIENT_H
#define MUTATECLIENT_H

#include "graphqlservice/GraphQLClient.h"
#include "graphqlservice/GraphQLParse.h"
#include "graphqlservice/GraphQLResponse.h"

#include "graphqlservice/internal/Version.h"

// Check if the library version is compatible with clientgen 4.2.0
static_assert(graphql::internal::MajorVersion == 4, "regenerate with clientgen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 2, "regenerate with clientgen: minor version mismatch");

#include <optional>
#include <string>
#include <vector>

/// <summary>
/// Operation: mutation CompleteTaskMutation
/// </summary>
/// <code class="language-graphql">
/// # Copyright (c) Microsoft Corporation. All rights reserved.
/// # Licensed under the MIT License.
/// 
/// mutation CompleteTaskMutation($input: CompleteTaskInput = {id: "ZmFrZVRhc2tJZA==", isComplete: true, clientMutationId: "Hi There!"}, $skipClientMutationId: Boolean!) {
///   completedTask: completeTask(input: $input) {
///     completedTask: task {
///       completedTaskId: id
///       title
///       isComplete
///     }
///     clientMutationId @skip(if: $skipClientMutationId)
///   }
/// }
/// </code>
namespace graphql::client::mutation::CompleteTaskMutation {

// Return the original text of the request document.
const std::string& GetRequestText() noexcept;

// Return a pre-parsed, pre-validated request object.
const peg::ast& GetRequestObject() noexcept;

enum class [[nodiscard]] TaskState
{
	New,
	Started,
	Complete,
	Unassigned,
};

struct Variables
{
	struct CompleteTaskInput
	{
		response::IdType id {};
		std::optional<TaskState> testTaskState {};
		std::optional<bool> isComplete {};
		std::optional<std::string> clientMutationId {};
	};

	std::unique_ptr<CompleteTaskInput> input {};
	bool skipClientMutationId {};
};

response::Value serializeVariables(Variables&& variables);

struct Response
{
	struct completedTask_CompleteTaskPayload
	{
		struct completedTask_Task
		{
			response::IdType completedTaskId {};
			std::optional<std::string> title {};
			bool isComplete {};
		};

		std::optional<completedTask_Task> completedTask {};
		std::optional<std::string> clientMutationId {};
	};

	completedTask_CompleteTaskPayload completedTask {};
};

Response parseResponse(response::Value&& response);

} // namespace graphql::client::mutation::CompleteTaskMutation

#endif // MUTATECLIENT_H
