FROM shillaker/cpp-base

RUN apt-add-repository ppa:ansible/ansible
RUN apt-get update
RUN apt-get install -y ansible

# Run Ansible set-up
COPY . /faasm/code
WORKDIR /faasm/code/ansible
RUN ansible-playbook libs.yml

CMD /bin/bash