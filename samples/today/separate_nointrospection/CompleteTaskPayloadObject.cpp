// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "TodayObjects.h"

#include "graphqlservice/introspection/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::today {
namespace object {

CompleteTaskPayload::CompleteTaskPayload(std::unique_ptr<Concept>&& pimpl) noexcept
	: service::Object{ getTypeNames(), getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

service::TypeNames CompleteTaskPayload::getTypeNames() const noexcept
{
	return {
		"CompleteTaskPayload"
	};
}

service::ResolverMap CompleteTaskPayload::getResolvers() const noexcept
{
	return {
		{ R"gql(task)gql"sv, [this](service::ResolverParams&& params) { return resolveTask(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(clientMutationId)gql"sv, [this](service::ResolverParams&& params) { return resolveClientMutationId(std::move(params)); } }
	};
}

void CompleteTaskPayload::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void CompleteTaskPayload::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

service::AwaitableResolver CompleteTaskPayload::resolveTask(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getTask(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Task>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver CompleteTaskPayload::resolveClientMutationId(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getClientMutationId(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<std::string>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver CompleteTaskPayload::resolve_typename(service::ResolverParams&& params) const
{
	return service::ModifiedResult<std::string>::convert(std::string{ R"gql(CompleteTaskPayload)gql" }, std::move(params));
}

} // namespace object

void AddCompleteTaskPayloadDetails(const std::shared_ptr<schema::ObjectType>& typeCompleteTaskPayload, const std::shared_ptr<schema::Schema>& schema)
{
	typeCompleteTaskPayload->AddFields({
		schema::Field::Make(R"gql(task)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType("Task")),
		schema::Field::Make(R"gql(clientMutationId)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType("String"))
	});
}

} // namespace graphql::today
