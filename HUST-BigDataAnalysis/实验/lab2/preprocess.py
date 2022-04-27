import pandas as pd
emails = pd.read_csv('datasets/Emails.csv', low_memory=False)
aliases = {}
f = pd.read_csv('datasets/Aliases.csv', low_memory=False)
for index, row in f.iterrows():
    aliases[row['Alias']] = row['PersonId']
persons = {}
persons_ = {}
f = pd.read_csv('datasets/Persons.csv', low_memory=False)
for index, row in f.iterrows():
    persons[row['Id']] = row['Name']
    persons_[row['Name']] = row['Id']

def name_uni(name):
    """
    """
    name = str(name).lower()
    name = name.replace(',', '')
    if name in aliases.keys():
        return persons[aliases[name]]
    return name

def generate_egdes():
    """
    """
    emails.MetadataFrom = emails.MetadataFrom.apply(name_uni)
    emails.MetadataTo = emails.MetadataTo.apply(name_uni)

    sent_receive = {}
    for index, row in emails.iterrows():
        if row['MetadataFrom'] not in persons_.keys() or row['MetadataTo'] not in persons_.keys():
            continue
        sent_receive[index] = [persons_[row['MetadataFrom']], persons_[row['MetadataTo']]]
    # print(sent_receive)
    name = ['sent_id', 'receive_id']
    data = pd.DataFrame.from_dict(sent_receive, orient='index', columns=name)
    data.to_csv('sent_receive.csv')

if __name__ == '__main__':
    generate_egdes()