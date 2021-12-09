// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef SUBSCRIPTIONOBJECT_H
#define SUBSCRIPTIONOBJECT_H

#include "ValidationSchema.h"

namespace graphql::validation::object {
namespace methods::SubscriptionHas {

template <class TImpl>
concept getNewMessageWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::shared_ptr<Message>> { impl.getNewMessage(std::move(params)) } };
};

template <class TImpl>
concept getNewMessage = requires (TImpl impl) 
{
	{ service::FieldResult<std::shared_ptr<Message>> { impl.getNewMessage() } };
};

template <class TImpl>
concept getDisallowedSecondRootFieldWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<bool> { impl.getDisallowedSecondRootField(std::move(params)) } };
};

template <class TImpl>
concept getDisallowedSecondRootField = requires (TImpl impl) 
{
	{ service::FieldResult<bool> { impl.getDisallowedSecondRootField() } };
};

template <class TImpl>
concept beginSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.beginSelectionSet(params) };
};

template <class TImpl>
concept endSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.endSelectionSet(params) };
};

} // namespace methods::SubscriptionHas

class Subscription
	: public service::Object
{
private:
	service::AwaitableResolver resolveNewMessage(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveDisallowedSecondRootField(service::ResolverParams&& params) const;

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		virtual service::FieldResult<std::shared_ptr<Message>> getNewMessage(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<bool> getDisallowedSecondRootField(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::FieldResult<std::shared_ptr<Message>> getNewMessage(service::FieldParams&& params) const final
		{
			if constexpr (methods::SubscriptionHas::getNewMessageWithParams<T>)
			{
				return { _pimpl->getNewMessage(std::move(params)) };
			}
			else if constexpr (methods::SubscriptionHas::getNewMessage<T>)
			{
				return { _pimpl->getNewMessage() };
			}
			else
			{
				throw std::runtime_error(R"ex(Subscription::getNewMessage is not implemented)ex");
			}
		}

		service::FieldResult<bool> getDisallowedSecondRootField(service::FieldParams&& params) const final
		{
			if constexpr (methods::SubscriptionHas::getDisallowedSecondRootFieldWithParams<T>)
			{
				return { _pimpl->getDisallowedSecondRootField(std::move(params)) };
			}
			else if constexpr (methods::SubscriptionHas::getDisallowedSecondRootField<T>)
			{
				return { _pimpl->getDisallowedSecondRootField() };
			}
			else
			{
				throw std::runtime_error(R"ex(Subscription::getDisallowedSecondRootField is not implemented)ex");
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::SubscriptionHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::SubscriptionHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Subscription(std::unique_ptr<Concept>&& pimpl) noexcept;

	service::TypeNames getTypeNames() const noexcept;
	service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<Concept> _pimpl;

public:
	template <class T>
	Subscription(std::shared_ptr<T> pimpl) noexcept
		: Subscription { std::unique_ptr<Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}
};

} // namespace graphql::validation::object

#endif // SUBSCRIPTIONOBJECT_H
