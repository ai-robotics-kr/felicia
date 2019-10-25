import grpc

import felicia_py


class Client(felicia_py.rpc._Client):
    @classmethod
    def new_stub(cls, channel):
        raise NotImplementedError("new_stub not implemented")

    def connect(self, ip_endpoint, callback):
        self.channel = grpc.insecure_channel(
            "{}:{}".format(ip_endpoint.ip, ip_endpoint.port))
        self.stub = self.new_stub(self.channel)
        callback(felicia_py.Status.OK())

    def shutdown(self):
        self.channel.close()
        return felicia_py.Status.OK()
