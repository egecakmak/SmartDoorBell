import json, os


def add_authorized_user(name):
    if is_authorized(name):
        return False
    else:
        data = {}
        data['authorized_users'] = []
        data['authorized_users'].append({
            'name': name
        })

        if os.path.isfile('./authorizations.txt'):
            data = copy_json(data)

        write_json(data)
        return True


def delete_authorized_user(name):
    if os.path.isfile('./authorizations.txt'):
        with open('authorizations.txt') as input:
            input_data = json.load(input)
            for e in input_data['authorized_users']:
                if name == e['name']:
                    del e['name'] ## fix this
                    write_json(input_data)
                    return True
    return False


def is_authorized(name):
    if os.path.isfile('./authorizations.txt'):
        with open('authorizations.txt') as input:
            input_data = json.load(input)
            for e in input_data['authorized_users']:
                if name == e['name']:
                    return True
            return False
    else:
        return False


def copy_json(data):
    with open('authorizations.txt') as input:
        input_data = json.load(input)
        for e in input_data['authorized_users']:
            data['authorized_users'].append(e)
        return data


def write_json(data):
    with open('authorizations.txt', 'w') as output:
        json.dump(data, output)


def print_operations():
    print ("Operations")
    print ("1) Authorize a user")
    print ("2) Revoke authentication of a user")
    print ("3) Check if a user is authorized")
    print ("4) Quit")
    print ("Please choose an operation to continue.")
