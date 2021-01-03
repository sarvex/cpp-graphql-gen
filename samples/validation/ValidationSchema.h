// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ifndef VALIDATIONSCHEMA_H
#define VALIDATIONSCHEMA_H

#include "graphqlservice/GraphQLSchema.h"
#include "graphqlservice/GraphQLService.h"

// Check if the library version is compatible with schemagen 3.4.1
static_assert(graphql::internal::MajorVersion == 3, "regenerate with schemagen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 4, "regenerate with schemagen: minor version mismatch");

#include <memory>
#include <string>
#include <vector>

namespace graphql {
namespace validation {

enum class DogCommand
{
	SIT,
	DOWN,
	HEEL
};

enum class CatCommand
{
	JUMP
};

struct ComplexInput
{
	std::optional<response::StringType> name;
	std::optional<response::StringType> owner;
};

namespace object {

class Query;
class Dog;
class Alien;
class Human;
class Cat;
class Mutation;
class MutateDogResult;
class Subscription;
class Message;
class Arguments;

} /* namespace object */

struct Sentient;
struct Pet;

struct Sentient
{
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const = 0;
};

struct Pet
{
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const = 0;
};

namespace object {

class Query
	: public service::Object
{
protected:
	explicit Query();

public:
	virtual service::FieldResult<std::shared_ptr<Dog>> getDog(service::FieldParams&& params) const;
	virtual service::FieldResult<std::shared_ptr<Human>> getHuman(service::FieldParams&& params) const;
	virtual service::FieldResult<std::shared_ptr<service::Object>> getPet(service::FieldParams&& params) const;
	virtual service::FieldResult<std::shared_ptr<service::Object>> getCatOrDog(service::FieldParams&& params) const;
	virtual service::FieldResult<std::shared_ptr<Arguments>> getArguments(service::FieldParams&& params) const;
	virtual service::FieldResult<std::shared_ptr<Dog>> getFindDog(service::FieldParams&& params, std::optional<ComplexInput>&& complexArg) const;
	virtual service::FieldResult<std::optional<response::BooleanType>> getBooleanList(service::FieldParams&& params, std::optional<std::vector<response::BooleanType>>&& booleanListArgArg) const;

private:
	std::future<service::ResolverResult> resolveDog(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveHuman(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolvePet(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveCatOrDog(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveArguments(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveFindDog(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveBooleanList(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolve_schema(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolve_type(service::ResolverParams&& params);

	std::shared_ptr<schema::Schema> _schema;
};

class Dog
	: public service::Object
	, public Pet
{
protected:
	explicit Dog();

public:
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const override;
	virtual service::FieldResult<std::optional<response::StringType>> getNickname(service::FieldParams&& params) const;
	virtual service::FieldResult<std::optional<response::IntType>> getBarkVolume(service::FieldParams&& params) const;
	virtual service::FieldResult<response::BooleanType> getDoesKnowCommand(service::FieldParams&& params, DogCommand&& dogCommandArg) const;
	virtual service::FieldResult<response::BooleanType> getIsHousetrained(service::FieldParams&& params, std::optional<response::BooleanType>&& atOtherHomesArg) const;
	virtual service::FieldResult<std::shared_ptr<Human>> getOwner(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveName(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveNickname(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveBarkVolume(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveDoesKnowCommand(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveIsHousetrained(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveOwner(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class Alien
	: public service::Object
	, public Sentient
{
protected:
	explicit Alien();

public:
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const override;
	virtual service::FieldResult<std::optional<response::StringType>> getHomePlanet(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveName(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveHomePlanet(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class Human
	: public service::Object
	, public Sentient
{
protected:
	explicit Human();

public:
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const override;
	virtual service::FieldResult<std::vector<std::shared_ptr<service::Object>>> getPets(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveName(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolvePets(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class Cat
	: public service::Object
	, public Pet
{
protected:
	explicit Cat();

public:
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const override;
	virtual service::FieldResult<std::optional<response::StringType>> getNickname(service::FieldParams&& params) const;
	virtual service::FieldResult<response::BooleanType> getDoesKnowCommand(service::FieldParams&& params, CatCommand&& catCommandArg) const;
	virtual service::FieldResult<std::optional<response::IntType>> getMeowVolume(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveName(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveNickname(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveDoesKnowCommand(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveMeowVolume(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class Mutation
	: public service::Object
{
protected:
	explicit Mutation();

public:
	virtual service::FieldResult<std::shared_ptr<MutateDogResult>> applyMutateDog(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveMutateDog(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class MutateDogResult
	: public service::Object
{
protected:
	explicit MutateDogResult();

public:
	virtual service::FieldResult<response::IdType> getId(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveId(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class Subscription
	: public service::Object
{
protected:
	explicit Subscription();

public:
	virtual service::FieldResult<std::shared_ptr<Message>> getNewMessage(service::FieldParams&& params) const;
	virtual service::FieldResult<response::BooleanType> getDisallowedSecondRootField(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveNewMessage(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveDisallowedSecondRootField(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class Message
	: public service::Object
{
protected:
	explicit Message();

public:
	virtual service::FieldResult<std::optional<response::StringType>> getBody(service::FieldParams&& params) const;
	virtual service::FieldResult<response::IdType> getSender(service::FieldParams&& params) const;

private:
	std::future<service::ResolverResult> resolveBody(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveSender(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

class Arguments
	: public service::Object
{
protected:
	explicit Arguments();

public:
	virtual service::FieldResult<response::IntType> getMultipleReqs(service::FieldParams&& params, response::IntType&& xArg, response::IntType&& yArg) const;
	virtual service::FieldResult<std::optional<response::BooleanType>> getBooleanArgField(service::FieldParams&& params, std::optional<response::BooleanType>&& booleanArgArg) const;
	virtual service::FieldResult<std::optional<response::FloatType>> getFloatArgField(service::FieldParams&& params, std::optional<response::FloatType>&& floatArgArg) const;
	virtual service::FieldResult<std::optional<response::IntType>> getIntArgField(service::FieldParams&& params, std::optional<response::IntType>&& intArgArg) const;
	virtual service::FieldResult<response::BooleanType> getNonNullBooleanArgField(service::FieldParams&& params, response::BooleanType&& nonNullBooleanArgArg) const;
	virtual service::FieldResult<std::optional<std::vector<response::BooleanType>>> getNonNullBooleanListField(service::FieldParams&& params, std::optional<std::vector<response::BooleanType>>&& nonNullBooleanListArgArg) const;
	virtual service::FieldResult<std::optional<std::vector<std::optional<response::BooleanType>>>> getBooleanListArgField(service::FieldParams&& params, std::vector<std::optional<response::BooleanType>>&& booleanListArgArg) const;
	virtual service::FieldResult<response::BooleanType> getOptionalNonNullBooleanArgField(service::FieldParams&& params, response::BooleanType&& optionalBooleanArgArg) const;

private:
	std::future<service::ResolverResult> resolveMultipleReqs(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveBooleanArgField(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveFloatArgField(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveIntArgField(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveNonNullBooleanArgField(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveNonNullBooleanListField(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveBooleanListArgField(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveOptionalNonNullBooleanArgField(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

} /* namespace object */

class Operations
	: public service::Request
{
public:
	explicit Operations(std::shared_ptr<object::Query> query, std::shared_ptr<object::Mutation> mutation, std::shared_ptr<object::Subscription> subscription);

private:
	std::shared_ptr<object::Query> _query;
	std::shared_ptr<object::Mutation> _mutation;
	std::shared_ptr<object::Subscription> _subscription;
};

std::shared_ptr<schema::Schema> GetSchema();

} /* namespace validation */
} /* namespace graphql */

#endif // VALIDATIONSCHEMA_H
