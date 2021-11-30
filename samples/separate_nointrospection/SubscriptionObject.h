// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef SUBSCRIPTIONOBJECT_H
#define SUBSCRIPTIONOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {

class Subscription
	: public service::Object
{
private:
	service::AwaitableResolver resolveNextAppointmentChange(service::ResolverParams&& params);
	service::AwaitableResolver resolveNodeChange(service::ResolverParams&& params);

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params);

	struct Concept
	{
		virtual service::FieldResult<std::shared_ptr<Appointment>> getNextAppointmentChange(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<std::shared_ptr<service::Object>> getNodeChange(service::FieldParams&& params, response::IdType&& idArg) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::FieldResult<std::shared_ptr<Appointment>> getNextAppointmentChange(service::FieldParams&& params) const final
		{
			return _pimpl->getNextAppointmentChange(std::move(params));
		}

		service::FieldResult<std::shared_ptr<service::Object>> getNodeChange(service::FieldParams&& params, response::IdType&& idArg) const final
		{
			return _pimpl->getNodeChange(std::move(params), std::move(idArg));
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Subscription(std::unique_ptr<Concept>&& pimpl);

	const std::unique_ptr<Concept> _pimpl;

public:
	template <class T>
	Subscription(std::shared_ptr<T> pimpl)
		: Subscription { std::make_unique<Model<T>>(std::move(pimpl)) }
	{
	}

	~Subscription();
};

} // namespace graphql::today::object

#endif // SUBSCRIPTIONOBJECT_H
