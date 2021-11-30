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

Query::Query(std::unique_ptr<Concept>&& pimpl)
	: service::Object({
		"Query"
	}, {
		{ R"gql(node)gql"sv, [this](service::ResolverParams&& params) { return resolveNode(std::move(params)); } },
		{ R"gql(tasks)gql"sv, [this](service::ResolverParams&& params) { return resolveTasks(std::move(params)); } },
		{ R"gql(__type)gql"sv, [this](service::ResolverParams&& params) { return resolve_type(std::move(params)); } },
		{ R"gql(nested)gql"sv, [this](service::ResolverParams&& params) { return resolveNested(std::move(params)); } },
		{ R"gql(anyType)gql"sv, [this](service::ResolverParams&& params) { return resolveAnyType(std::move(params)); } },
		{ R"gql(__schema)gql"sv, [this](service::ResolverParams&& params) { return resolve_schema(std::move(params)); } },
		{ R"gql(expensive)gql"sv, [this](service::ResolverParams&& params) { return resolveExpensive(std::move(params)); } },
		{ R"gql(tasksById)gql"sv, [this](service::ResolverParams&& params) { return resolveTasksById(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(appointments)gql"sv, [this](service::ResolverParams&& params) { return resolveAppointments(std::move(params)); } },
		{ R"gql(unreadCounts)gql"sv, [this](service::ResolverParams&& params) { return resolveUnreadCounts(std::move(params)); } },
		{ R"gql(testTaskState)gql"sv, [this](service::ResolverParams&& params) { return resolveTestTaskState(std::move(params)); } },
		{ R"gql(unimplemented)gql"sv, [this](service::ResolverParams&& params) { return resolveUnimplemented(std::move(params)); } },
		{ R"gql(appointmentsById)gql"sv, [this](service::ResolverParams&& params) { return resolveAppointmentsById(std::move(params)); } },
		{ R"gql(unreadCountsById)gql"sv, [this](service::ResolverParams&& params) { return resolveUnreadCountsById(std::move(params)); } }
	})
	, _schema(GetSchema())
	, _pimpl(std::move(pimpl))
{
}

Query::~Query()
{
}

service::AwaitableResolver Query::resolveNode(service::ResolverParams&& params)
{
	auto argId = service::ModifiedArgument<response::IdType>::require("id", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getNode(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argId));
	resolverLock.unlock();

	return service::ModifiedResult<service::Object>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveAppointments(service::ResolverParams&& params)
{
	auto argFirst = service::ModifiedArgument<response::IntType>::require<service::TypeModifier::Nullable>("first", params.arguments);
	auto argAfter = service::ModifiedArgument<response::Value>::require<service::TypeModifier::Nullable>("after", params.arguments);
	auto argLast = service::ModifiedArgument<response::IntType>::require<service::TypeModifier::Nullable>("last", params.arguments);
	auto argBefore = service::ModifiedArgument<response::Value>::require<service::TypeModifier::Nullable>("before", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getAppointments(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argFirst), std::move(argAfter), std::move(argLast), std::move(argBefore));
	resolverLock.unlock();

	return service::ModifiedResult<AppointmentConnection>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveTasks(service::ResolverParams&& params)
{
	auto argFirst = service::ModifiedArgument<response::IntType>::require<service::TypeModifier::Nullable>("first", params.arguments);
	auto argAfter = service::ModifiedArgument<response::Value>::require<service::TypeModifier::Nullable>("after", params.arguments);
	auto argLast = service::ModifiedArgument<response::IntType>::require<service::TypeModifier::Nullable>("last", params.arguments);
	auto argBefore = service::ModifiedArgument<response::Value>::require<service::TypeModifier::Nullable>("before", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getTasks(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argFirst), std::move(argAfter), std::move(argLast), std::move(argBefore));
	resolverLock.unlock();

	return service::ModifiedResult<TaskConnection>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveUnreadCounts(service::ResolverParams&& params)
{
	auto argFirst = service::ModifiedArgument<response::IntType>::require<service::TypeModifier::Nullable>("first", params.arguments);
	auto argAfter = service::ModifiedArgument<response::Value>::require<service::TypeModifier::Nullable>("after", params.arguments);
	auto argLast = service::ModifiedArgument<response::IntType>::require<service::TypeModifier::Nullable>("last", params.arguments);
	auto argBefore = service::ModifiedArgument<response::Value>::require<service::TypeModifier::Nullable>("before", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getUnreadCounts(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argFirst), std::move(argAfter), std::move(argLast), std::move(argBefore));
	resolverLock.unlock();

	return service::ModifiedResult<FolderConnection>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveAppointmentsById(service::ResolverParams&& params)
{
	const auto defaultArguments = []()
	{
		response::Value values(response::Type::Map);
		response::Value entry;

		entry = []()
		{
			response::Value elements(response::Type::List);
			response::Value entry;

			entry = response::Value(std::string(R"gql(ZmFrZUFwcG9pbnRtZW50SWQ=)gql"));
			elements.emplace_back(std::move(entry));
			return elements;
		}();
		values.emplace_back("ids", std::move(entry));

		return values;
	}();

	auto pairIds = service::ModifiedArgument<response::IdType>::find<service::TypeModifier::List>("ids", params.arguments);
	auto argIds = (pairIds.second
		? std::move(pairIds.first)
		: service::ModifiedArgument<response::IdType>::require<service::TypeModifier::List>("ids", defaultArguments));
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getAppointmentsById(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Appointment>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveTasksById(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getTasksById(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Task>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveUnreadCountsById(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getUnreadCountsById(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Folder>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveNested(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getNested(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<NestedType>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveUnimplemented(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getUnimplemented(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<response::StringType>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveExpensive(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getExpensive(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Expensive>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveTestTaskState(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getTestTaskState(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<TaskState>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveAnyType(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getAnyType(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<service::Object>::convert<service::TypeModifier::List, service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(Query)gql" }, std::move(params));
}

service::AwaitableResolver Query::resolve_schema(service::ResolverParams&& params)
{
	return service::ModifiedResult<service::Object>::convert(std::static_pointer_cast<service::Object>(std::make_shared<introspection::object::Schema>(std::make_shared<introspection::Schema>(_schema))), std::move(params));
}

service::AwaitableResolver Query::resolve_type(service::ResolverParams&& params)
{
	auto argName = service::ModifiedArgument<response::StringType>::require("name", params.arguments);
	const auto& baseType = _schema->LookupType(argName);
	std::shared_ptr<introspection::object::Type> result { baseType ? std::make_shared<introspection::object::Type>(std::make_shared<introspection::Type>(baseType)) : nullptr };

	return service::ModifiedResult<introspection::object::Type>::convert<service::TypeModifier::Nullable>(result, std::move(params));
}

} // namespace object

void AddQueryDetails(std::shared_ptr<schema::ObjectType> typeQuery, const std::shared_ptr<schema::Schema>& schema)
{
	typeQuery->AddFields({
		schema::Field::Make(R"gql(node)gql"sv, R"md([Object Identification](https://facebook.github.io/relay/docs/en/graphql-server-specification.html#object-identification))md"sv, std::nullopt, schema->LookupType("Node"), {
			schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(appointments)gql"sv, R"md(Appointments [Connection](https://facebook.github.io/relay/docs/en/graphql-server-specification.html#connections))md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("AppointmentConnection")), {
			schema::InputValue::Make(R"gql(first)gql"sv, R"md()md"sv, schema->LookupType("Int"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(after)gql"sv, R"md()md"sv, schema->LookupType("ItemCursor"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(last)gql"sv, R"md()md"sv, schema->LookupType("Int"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(before)gql"sv, R"md()md"sv, schema->LookupType("ItemCursor"), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(tasks)gql"sv, R"md(Tasks [Connection](https://facebook.github.io/relay/docs/en/graphql-server-specification.html#connections))md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("TaskConnection")), {
			schema::InputValue::Make(R"gql(first)gql"sv, R"md()md"sv, schema->LookupType("Int"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(after)gql"sv, R"md()md"sv, schema->LookupType("ItemCursor"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(last)gql"sv, R"md()md"sv, schema->LookupType("Int"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(before)gql"sv, R"md()md"sv, schema->LookupType("ItemCursor"), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(unreadCounts)gql"sv, R"md(Folder unread counts [Connection](https://facebook.github.io/relay/docs/en/graphql-server-specification.html#connections))md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("FolderConnection")), {
			schema::InputValue::Make(R"gql(first)gql"sv, R"md()md"sv, schema->LookupType("Int"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(after)gql"sv, R"md()md"sv, schema->LookupType("ItemCursor"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(last)gql"sv, R"md()md"sv, schema->LookupType("Int"), R"gql()gql"sv),
			schema::InputValue::Make(R"gql(before)gql"sv, R"md()md"sv, schema->LookupType("ItemCursor"), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(appointmentsById)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Appointment"))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")))), R"gql(["ZmFrZUFwcG9pbnRtZW50SWQ="])gql"sv)
		}),
		schema::Field::Make(R"gql(tasksById)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Task"))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")))), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(unreadCountsById)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("Folder"))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")))), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(nested)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("NestedType"))),
		schema::Field::Make(R"gql(unimplemented)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("String"))),
		schema::Field::Make(R"gql(expensive)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Expensive"))))),
		schema::Field::Make(R"gql(testTaskState)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("TaskState"))),
		schema::Field::Make(R"gql(anyType)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->LookupType("UnionType"))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")))), R"gql()gql"sv)
		})
	});
}

} // namespace graphql::today
