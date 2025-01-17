// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "ResourceObject.h"

#include "graphqlservice/internal/Schema.h"

#include "graphqlservice/introspection/IntrospectionSchema.h"

using namespace std::literals;

namespace graphql::validation {
namespace object {

Resource::Resource(std::unique_ptr<const Concept>&& pimpl) noexcept
	: service::Object { pimpl->getTypeNames(), pimpl->getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

void Resource::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void Resource::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

} // namespace object

void AddResourceDetails(const std::shared_ptr<schema::InterfaceType>& typeResource, const std::shared_ptr<schema::Schema>& schema)
{
	typeResource->AddInterfaces({
		std::static_pointer_cast<const schema::InterfaceType>(schema->LookupType(R"gql(Node)gql"sv))
	});
	typeResource->AddFields({
		schema::Field::Make(R"gql(id)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(ID)gql"sv))),
		schema::Field::Make(R"gql(url)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(String)gql"sv))
	});
}

} // namespace graphql::validation
