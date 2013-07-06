#!env python2

import rpc_pb2
import interface_pb2
import zmq

context = zmq.Context()


def request(server, method, params):
    socket = context.socket(zmq.REQ)
    socket.connect(server)

    request = rpc_pb2.Request()
    request.method = method
    request.param = params

    socket.send(request.SerializeToString())
    reply = socket.recv()
    response = rpc_pb2.Response.FromString(reply)

    return response.data


def pbrequest(server, method, params):
    params = params.SerializeToString()
    response = request(server, method, params)
    return response


class SAEClient(object):
    def __init__(self, endpoint):
        self.endpoint = endpoint

    def echo_test(self, s):
        return request(self.endpoint, "echo_test", s)

    def entity_search(self, dataset, query):
        r = interface_pb2.EntitySearchRequest()
        r.dataset = dataset
        r.query = query
        response = pbrequest(self.endpoint, "EntitySearch", r)
        er = interface_pb2.EntitySearchResponse()
        er.ParseFromString(response)
        return er


def main():
    c = SAEClient("tcp://localhost:40111")
    print c.entity_search("academic", "project develop")


if __name__ == "__main__":
    main()
