<clientConfig version="1.1">
  <emailProvider id="peptest.ch">
    <domain>peptest.ch</domain>
    <displayName>pEp Test</displayName>
    <displayShortName>pEp Test</displayShortName>
    <incomingServer type="imap">
      <hostname>peptest.ch</hostname>
      <port>993</port>
      <socketType>SSL</socketType>
      <username>%EMAILADDRESS%</username>
      <authentication>password-cleartext</authentication>
    </incomingServer>
    <outgoingServer type="smtp">
      <hostname>peptest.ch</hostname>
      <port>587</port>
      <socketType>STARTTLS</socketType>
      <username>%EMAILADDRESS%</username>
      <authentication>password-cleartext</authentication>
    </outgoingServer>
  </emailProvider>
</clientConfig>
