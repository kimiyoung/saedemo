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
    request.param = params.SerializeToString()

    socket.send(request.SerializeToString())
    reply = socket.recv()

    response = rpc_pb2.Response.FromString(reply)
    return response


class DataCenterClient(object):
    def __init__(self, endpoint):
        self.endpoint = endpoint

    def searchAuthors(self, query, offset=0, count=20,
                      returned_fields=["naid", "names", "email"]):
        params = interface_pb2.StringQueryParams()
        params.query = query
        params.offset = offset
        params.count = count
        params.returned_fields.extend(returned_fields)
        method = "AuthorService_searchAuthors"
        response = request(self.endpoint, method, params)
        result = interface_pb2.AuthorResult.FromString(response.data)
        return result

    def searchConferences(self, query, offset=0, count=20):
        params = interface_pb2.StringQueryParams()
        params.query = query
        params.offset = offset
        params.count = count
        method = "ConfService_searchConferences"
        response = request(self.endpoint, method, params)
        result = interface_pb2.ConferenceResult.FromString(response.data)
        return result

    def searchPublications(self, query, offset=0, count=20,
                           returned_fields=["title", "id"]):
        params = interface_pb2.StringQueryParams()
        params.query = query
        params.offset = offset
        params.count = count
        params.returned_fields.extend(returned_fields)
        method = "PubService_searchPublications"
        response = request(self.endpoint, method, params)
        result = interface_pb2.PublicationResult.FromString(response.data)
        return result


def main():
    c = DataCenterClient("tcp://10.1.1.211:32011")
    print c.searchAuthors("data mining")


if __name__ == "__main__":
    main()
