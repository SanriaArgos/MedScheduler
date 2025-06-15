"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function SeniorAdminHospitalsPage() {
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [success, setSuccess] = useState("");
    const router = useRouter();

    // Данные администратора
    const [adminId, setAdminId] = useState(null);

    // Список больниц
    const [hospitals, setHospitals] = useState([]);

    // Состояние для добавления больницы
    const [isAddingHospital, setIsAddingHospital] = useState(false);
    const [newHospital, setNewHospital] = useState({
        region: "",
        settlementType: "",
        settlementName: "",
        street: "",
        house: "",
        fullName: "",
        adminId: "" // Оставляем adminId для выбора из списка
    });

    // Списки для выбора
    // Удаляем regions, settlementTypes, settlementNames состояния, так как они будут текстовыми инпутами
    // const [regions, setRegions] = useState([]); 
    // const [settlementTypes, setSettlementTypes] = useState([]);
    // const [settlementNames, setSettlementNames] = useState([]);
    const [juniorAdmins, setJuniorAdmins] = useState([]);

    // Поиск больниц
    const [searchQuery, setSearchQuery] = useState("");
    const [filteredHospitals, setFilteredHospitals] = useState([]);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'senior') {
            router.push('/login');
            return;
        }

        setAdminId(userData.userId);

        // Загружаем списки для выбора и список больниц
        // Удаляем вызовы fetchRegions, fetchSettlementTypes, fetchSettlementNames
        // fetchRegions();
        // fetchSettlementTypes();
        // fetchSettlementNames();
        fetchJuniorAdmins();
        fetchHospitals();
    }, [router]);

    useEffect(() => {
        // Фильтрация больниц при изменении поискового запроса
        if (searchQuery) {
            const lowercaseQuery = searchQuery.toLowerCase();
            const filtered = hospitals.filter(
                hospital =>
                    hospital.full_name.toLowerCase().includes(lowercaseQuery) ||
                    hospital.region.toLowerCase().includes(lowercaseQuery) ||
                    hospital.settlement_name.toLowerCase().includes(lowercaseQuery) ||
                    hospital.settlement_type.toLowerCase().includes(lowercaseQuery) ||
                    hospital.street.toLowerCase().includes(lowercaseQuery)
            );
            setFilteredHospitals(filtered);
        } else {
            setFilteredHospitals(hospitals);
        }
    }, [searchQuery, hospitals]);    const fetchJuniorAdmins = async () => {
        try {
            // Запрос к API для получения списка всех пользователей
            const response = await fetch('https://api.medscheduler.ru/get_users');
            const data = await response.json();

            if (response.ok && data.success) {
                // Фильтруем только младших администраторов и форматируем данные
                const juniorAdmins = data.users.filter(user => user.user_type === 'junior administrator');
                const formattedAdmins = juniorAdmins.map(admin => ({
                    user_id: admin.id, // Используем id вместо user_id
                    full_name: `${admin.last_name} ${admin.first_name} ${admin.patronymic || ""}`,
                    phone: admin.phone
                }));
                setJuniorAdmins(formattedAdmins);
            } else {
                console.error("API error:", data.error);
                setError(data.error || "Ошибка при получении списка младших администраторов");
                setJuniorAdmins([]);
            }
        } catch (err) {
            console.error("Error fetching junior admins:", err);
            setError("Не удалось загрузить список младших администраторов");
            setJuniorAdmins([]);
        }
    };    const fetchHospitals = async () => {
        try {
            // Запрос к API для получения списка больниц
            const hospitalsResponse = await fetch('https://api.medscheduler.ru/get_hospitals');
            const hospitalsData = await hospitalsResponse.json();

            if (!hospitalsResponse.ok || !hospitalsData.success) {
                setError(hospitalsData.error || "Ошибка при получении списка больниц");
                return;
            }

            // Запрос к API для получения списка пользователей
            const usersResponse = await fetch('https://api.medscheduler.ru/get_users');
            const usersData = await usersResponse.json();
            
            let adminsMap = {};            if (usersResponse.ok && usersData.success) {
                // Создаем карту администраторов по id
                usersData.users
                    .filter(user => user.user_type === 'junior administrator')                    .forEach(admin => {
                        adminsMap[String(admin.id)] = `${admin.last_name} ${admin.first_name} ${admin.patronymic || ""}`.trim();
                    });
            }

            // Обогащаем данные больниц именами администраторов            
            const enrichedHospitals = hospitalsData.hospitals.map(hospital => {
                const adminId = String(hospital.administrator_id);
                const adminName = adminsMap[adminId] || null;
                return {
                    ...hospital,
                    admin_name: adminName
                };
            });

            setHospitals(enrichedHospitals);
            setFilteredHospitals(enrichedHospitals);
        } catch (err) {
            console.error("Error fetching hospitals:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    const handleInputChange = (e) => {
        const { name, value } = e.target;
        setNewHospital({
            ...newHospital,
            [name]: value
        });
    };

    const handleAddHospital = async (e) => {
        e.preventDefault();
        setError("");
        setSuccess("");

        // Валидация данных
        if (!newHospital.region || !newHospital.settlementType || !newHospital.settlementName ||
            !newHospital.street || !newHospital.house || !newHospital.fullName || !newHospital.adminId) {
            setError("Пожалуйста, заполните все поля");
            return;
        }

        try {
            const response = await fetch('https://api.medscheduler.ru/add_hospital', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    region: newHospital.region,
                    settlement_type: newHospital.settlementType,
                    settlement_name: newHospital.settlementName,
                    street: newHospital.street,
                    house: newHospital.house,
                    full_name: newHospital.fullName,
                    admin_id: newHospital.adminId
                }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setSuccess("Больница успешно добавлена");

                // Сбрасываем форму
                setNewHospital({
                    region: "",
                    settlementType: "",
                    settlementName: "",
                    street: "",
                    house: "",
                    fullName: "",
                    adminId: ""
                });

                // Обновляем список больниц
                fetchHospitals();

                // Скрываем форму через 2 секунды
                setTimeout(() => {
                    setIsAddingHospital(false);
                    setSuccess("");
                }, 10000);
            } else {
                setError(data.error || "Ошибка при добавлении больницы");
            }
        } catch (err) {
            console.error("Error adding hospital:", err);
            setError("Не удалось подключиться к серверу");
        }
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка данных...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Управление больницами</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}            {success && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {success}
                </div>
            )}

            {/* DEBUG INFO */}
            <div className="hidden bg-yellow-50 border border-yellow-200 text-yellow-800 px-4 py-3 rounded mb-4">
                <h3 className="font-bold">Debug Info:</h3>
                <p>Total hospitals: {hospitals.length}</p>
                <p>Hospitals with admins: {hospitals.filter(h => h.admin_name).length}</p>
                <div className="mt-2">
                    {hospitals.slice(0, 3).map(hospital => (
                        <div key={hospital.hospital_id}>
                            ID: {hospital.hospital_id}, Name: {hospital.full_name}, Admin ID: {hospital.administrator_id}, Admin: {hospital.admin_name || 'null'}
                        </div>
                    ))}
                </div>
            </div>

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">                <div className="flex flex-col md:flex-row justify-between md:items-center mb-6">
                    <h2 className="text-xl font-semibold text-main2">Список больниц</h2>
                    <div className="mt-4 md:mt-0">
                        {juniorAdmins.length > 0 ? (
                            <button
                                onClick={() => setIsAddingHospital(true)}
                                className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                            >
                                Добавить новую больницу
                            </button>
                        ) : (
                            <div className="text-sm text-gray-500">
                                <p>Сначала создайте младшего администратора</p>
                                <Link 
                                    href="/senior-admin/junior-admins" 
                                    className="text-main hover:text-main2 underline"
                                >
                                    Управление администраторами →
                                </Link>
                            </div>
                        )}
                    </div>
                </div>

                {/* Поиск больниц */}
                <div className="mb-6">
                    <label htmlFor="search" className="block text-sm font-medium text-gray-700 mb-1">
                        Поиск больниц
                    </label>
                    <input
                        type="text"
                        id="search"
                        value={searchQuery}
                        onChange={(e) => setSearchQuery(e.target.value)}
                        placeholder="Поиск по названию, региону или адресу"
                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                    />
                </div>                {isAddingHospital && (
                    <div className="mb-6 p-4 bg-gray-50 rounded-lg">
                        <h2 className="text-2xl font-semibold text-main2 mb-4">Добавить новую больницу</h2>
                        
                        {juniorAdmins.length === 0 && (
                            <div className="mb-4 p-3 bg-yellow-100 border border-yellow-400 text-yellow-700 rounded">
                                <p className="text-sm">
                                    <strong>Внимание:</strong> Для создания больницы необходим младший администратор. 
                                    Сначала создайте администратора в разделе "Управление администраторами".
                                </p>
                            </div>
                        )}
                        
                        <form onSubmit={handleAddHospital} className="space-y-4">
                            <div>
                                <label htmlFor="region" className="block text-sm font-medium text-gray-700">Регион</label>
                                <input
                                    type="text"
                                    name="region"
                                    id="region"
                                    value={newHospital.region}
                                    onChange={handleInputChange}
                                    className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500"
                                    required
                                />
                            </div>
                            <div>
                                <label htmlFor="settlementType" className="block text-sm font-medium text-gray-700">Тип населенного пункта</label>
                                <input
                                    type="text"
                                    name="settlementType"
                                    id="settlementType"
                                    value={newHospital.settlementType}
                                    onChange={handleInputChange}
                                    className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500"
                                    required
                                />
                            </div>
                            <div>
                                <label htmlFor="settlementName" className="block text-sm font-medium text-gray-700">Наименование населенного пункта</label>
                                <input
                                    type="text"
                                    name="settlementName"
                                    id="settlementName"
                                    value={newHospital.settlementName}
                                    onChange={handleInputChange}
                                    className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500"
                                    required
                                />
                            </div>
                            <div>
                                <label htmlFor="street" className="block text-sm font-medium text-gray-700">Улица *</label>
                                <input
                                    type="text"
                                    id="street"
                                    name="street"
                                    value={newHospital.street}
                                    onChange={handleInputChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    required
                                />
                            </div>
                            <div>
                                <label htmlFor="house" className="block text-sm font-medium text-gray-700">Номер дома *</label>
                                <input
                                    type="text"
                                    id="house"
                                    name="house"
                                    value={newHospital.house}
                                    onChange={handleInputChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    required
                                />
                            </div>
                            <div className="md:col-span-2">
                                <label htmlFor="fullName" className="block text-sm font-medium text-gray-700">Полное название больницы *</label>
                                <input
                                    type="text"
                                    id="fullName"
                                    name="fullName"
                                    value={newHospital.fullName}
                                    onChange={handleInputChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    required
                                />
                            </div>
                            <div className="md:col-span-2">
                                <label htmlFor="adminId" className="block text-sm font-medium text-gray-700">Младший администратор *</label>
                                <select
                                    id="adminId"
                                    name="adminId"
                                    value={newHospital.adminId}
                                    onChange={handleInputChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    required
                                >
                                    <option value="">Выберите администратора</option>
                                    {juniorAdmins.map((admin) => (
                                        <option key={admin.user_id} value={admin.user_id}>
                                            {admin.full_name} (тел: {admin.phone})
                                        </option>
                                    ))}
                                </select>
                            </div>                            <div className="flex gap-3">
                                <button
                                    type="submit"
                                    disabled={juniorAdmins.length === 0}
                                    className={`px-4 py-2 text-white rounded transition-colors ${
                                        juniorAdmins.length === 0 
                                            ? 'bg-gray-400 cursor-not-allowed' 
                                            : 'bg-main hover:bg-main2'
                                    }`}
                                >
                                    Добавить больницу
                                </button>
                                <button
                                    type="button"
                                    onClick={() => setIsAddingHospital(false)}
                                    className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                >
                                    Отмена
                                </button>
                            </div>
                        </form>
                    </div>
                )}

                {/* Список больниц */}
                <div>
                    {filteredHospitals.length === 0 ? (
                        <p className="text-gray-500 text-center py-4">Больницы не найдены</p>
                    ) : (
                        <div className="overflow-x-auto">
                            <table className="min-w-full divide-y divide-gray-200">
                                <thead className="bg-gray-50">
                                    <tr>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Название
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Адрес
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Администратор
                                        </th>
                                    </tr>
                                </thead>
                                <tbody className="bg-white divide-y divide-gray-200">
                                    {filteredHospitals.map((hospital) => (
                                        <tr key={hospital.hospital_id}>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm font-medium text-gray-900">
                                                    {hospital.full_name}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">
                                                    {hospital.region}, {hospital.settlement_type} {hospital.settlement_name}, ул. {hospital.street}, д. {hospital.house}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">
                                                    {/* В реальном приложении з��есь будет имя администратора */}
                                                    {hospital.admin_name || "Не назначен"}
                                                </div>
                                            </td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    )}
                </div>
            </div>

            <div className="flex justify-between">
                <Link href="/senior-admin/profile" className="text-main hover:text-main2 transition-colors">
                    ← Профиль
                </Link>
                <Link href="/senior-admin/junior-admins" className="text-main hover:text-main2 transition-colors">
                    Управление администраторами →
                </Link>
            </div>
        </div>
    );
}
