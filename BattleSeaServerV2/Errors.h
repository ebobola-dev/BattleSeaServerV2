#pragma once
enum class ServerError {
	FailedToSend,
	FailedToRecv,
	ConnectionClosed,
};