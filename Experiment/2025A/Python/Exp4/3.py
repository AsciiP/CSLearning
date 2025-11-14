with open('TeleAddressBook.txt', encoding='utf-8') as tele, open('EmailAddressBook.txt', encoding='utf-8') as email:
    tele_lines = tele.readlines()[1:]
    email_lines = email.readlines()[1:]
    
    tele_names = []
    tele_values = []

    for line in tele_lines:
        elements = line.split()
        tele_names.append(elements[0])
        tele_values.append(elements[1])
    
    email_names = []
    email_values = []

    for line in email_lines:
        elements = line.split()
        email_names.append(elements[0])
        email_values.append(elements[1])

    lines = []
    lines.append('姓名\t电话\t邮箱\n')

    for i in range(len(tele_names)):
        s = ''
        if tele_names[i] in email_names:
            j = email_names.index(tele_names[i])
            s = '\t'.join([tele_names[i], tele_values[i], email_values[j]]) + '\n'
        else:
            s = '\t'.join([tele_names[i], tele_values[i], '   N/A   ']) + '\n'
        lines.append(s)

    for i in range(len(email_names)):
        if email_names[i] not in tele_names:
            s = '\t'.join([email_names[i], '   N/A   ', email_values[i]]) + '\n'
            lines.append(s)
    
    with open('AddressBook.txt', 'w', encoding='utf-8') as merged:
        merged.writelines(lines)
    
    print('地址簿合并完成！')
